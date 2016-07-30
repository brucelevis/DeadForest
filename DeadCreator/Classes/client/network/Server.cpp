//
//  Server.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 5. 3..
//
//

#include <boost/bind.hpp>

#include "Server.hpp"
#include "Packet.hpp"
#include "GeneratedPackets.hpp"
#include "ParamLoader.hpp"

#include "cocos2d.h"
using namespace cocos2d;
using namespace realtrick::client;


void Server::connect()
{
    boost::asio::ip::tcp::resolver resolver(_io);
    auto endpoint = resolver.resolve( { Prm.getValueAsString("serverIP"), Prm.getValueAsString("serverPort") } );
    doConnect(endpoint);
    _thread = new boost::thread(boost::bind(&boost::asio::io_service::run, &_io));
}


void Server::close()
{
    _io.post([this]
    {
        closeSocket();
        enableConnected(false);
        _thread->join();
        CC_SAFE_DELETE(_thread);
    });
}


void Server::doConnect(boost::asio::ip::tcp::resolver::iterator endpoint)
{
    boost::asio::async_connect(_socket, endpoint,
                               [this](boost::system::error_code ec, boost::asio::ip::tcp::resolver::iterator)
                               {
                                   if ( !ec )
                                   {
                                       enableConnected(true);
                                       
                                       flatbuffers::FlatBufferBuilder builder;
                                       auto obj = fpacket::CreatePacketLogin(builder, cocos2d::UserDefault::getInstance()->getIntegerForKey("pid", -1));
                                       builder.Finish(obj);
                                       
                                       Packet* packet = new Packet();
                                       packet->encode(builder.GetBufferPointer(), builder.GetSize(), PacketType::LOGIN);
                                       Server::getInstance().write(packet);
                                       
                                       doReadHeader();
                                   }
                                   else
                                   {
                                       log("<Server::doConnect> error code: [%d]", ec.value());
                                   }
                               });
}


void Server::write(Packet* packet)
{
    bool writeInProgress = !_writeDeque.empty();
    _writeDeque.push_back(packet);
    if ( !writeInProgress ) doWrite();
    
    
}


void Server::doWrite()
{
    boost::asio::async_write(_socket,
                             boost::asio::buffer(_writeDeque.front()->data(), _writeDeque.front()->length()),
                             [this](boost::system::error_code ec, std::size_t length)
                             {
                                 if ( !ec )
                                 {
                                     Packet* packet = _writeDeque.front();
                                     packet->decode();
                                     log("<Server::doWrite> packet type: [%d]", static_cast<int>(packet->type()));
                                     
                                     if ( packet->type() == PacketType::MOVE_JOYSTICK )
                                     {
                                         auto obj = fpacket::GetPacketMoveJoystick(packet->body());
                                         log("id: %d, type: %s, dirx: %.f, diry: %.f", obj->id(), obj->is_touched() ? "began" : "ended", obj->dir_x(), obj->dir_y());
                                     }
                                     
                                     CC_SAFE_DELETE(packet);
                                     
                                     _writeDeque.pop_front();
                                     if ( !_writeDeque.empty() ) doWrite();
                                 }
                                 else
                                 {
                                     cocos2d::log("error code: %d", ec.value());
                                     closeSocket();
                                 }
                             });
}


void Server::doReadHeader()
{
    boost::asio::async_read(_socket, boost::asio::buffer(_buffer.data(), Packet::HEADER_LENGTH),
                            [this](boost::system::error_code ec, std::size_t /*length*/)
                            {
                                if ( !ec && _buffer.decode() )
                                {
                                    doReadBody();
                                }
                                else
                                {
                                    closeSocket();
                                }
                            });
}


void Server::doReadBody()
{
    boost::asio::async_read(_socket, boost::asio::buffer(_buffer.body(), _buffer.bodyLength()),
                            [this](boost::system::error_code ec, std::size_t /*length*/)
                            {
                                if ( !ec )
                                {
                                    Packet* packet = new Packet();
                                    memcpy(packet->data(), _buffer.data(), _buffer.length());
                                    enqueue(packet);
                                    
                                    doReadHeader();
                                }
                                else
                                {
                                    closeSocket();
                                }
                            });
    
}









