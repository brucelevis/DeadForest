//
//  Server.cpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 5. 3..
//
//

#include <boost/bind.hpp>

#include "Server.hpp"
#include "Packet.hpp"
#include "GeneratedPackets.hpp"
#include "ParamLoader.hpp"

using namespace realtrick;


void Server::connect()
{
    boost::asio::ip::tcp::resolver resolver(_io);
    auto endpoint = resolver.resolve( { Prm.getValueAsString("serverIP"), Prm.getValueAsString("serverPort") } );
    doConnect(endpoint);
    _thread = new boost::thread(boost::bind(&boost::asio::io_service::run, &_io));
}


void Server::close()
{
    _io.post([this] { closeSocket(); enableConnected(false); });
    _thread->join();
    
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
                                       auto obj = fpacket::CreatePacketLogin(builder, -1);//_pid 세팅 필요
                                       builder.Finish(obj);
                                       
                                       Packet* packet = new Packet();
                                       packet->encode(builder.GetBufferPointer(), builder.GetSize(), PacketType::LOGIN);
                                       Server::getInstance().write(packet);
                                       
                                       doReadHeader();
                                   }
                                   else
                                   {
                                       std::cout<<"<Server::doConnect> error code: ["<< ec.value()<<"]"<<std::endl;
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
                                     std::cout<<"<<Server::doWrite> packet type: ["<< static_cast<int>(packet->type()) <<"]"<<std::endl;
                                     
                                     if ( packet->type() == PacketType::MOVE_JOYSTICK )
                                     {
                                         auto obj = fpacket::GetPacketMoveJoystick(packet->body());
                                         
                                         std::cout<<"id: "<<obj->id()<<"type:"<<(obj->is_touched() ? "began" : "ended")<<"dirx: "<<obj->dir_x()<<"diry: "<<obj->dir_y()<<std::endl;
                                         
                                     }
                                     
                                     //CC_SAFE_DELETE(packet);
                                     
                                     _writeDeque.pop_front();
                                     if ( !_writeDeque.empty() ) doWrite();
                                 }
                                 else
                                 {
                                     std::cout<<"error code: "<< ec.value()<<std::endl;
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









