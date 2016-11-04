//
//  GameServer.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 5. 3..
//
//

#include "GameServer.hpp"
#include "Packet.hpp"
#include "GeneratedPackets.hpp"
#include "ParamLoader.hpp"
#include "cocos2d.h"
using namespace cocos2d;
using namespace realtrick::client;


GameServer::GameServer() : _socket(_io), _queue(512), _isConnected(false)
{
}


GameServer::~GameServer()
{
    close();
}


void GameServer::connect(const std::string& ip, const std::string& port)
{
    boost::asio::ip::tcp::resolver resolver(_io);
    auto endpoint = resolver.resolve( { ip, port } );
    boost::asio::async_connect(_socket, endpoint,
                               [this, ip, port] (boost::system::error_code ec, boost::asio::ip::tcp::resolver::iterator)
                               {
                                   if ( !ec )
                                   {
                                       log("GameServer::doConnet> connected!");
                                       doReadHeader();
                                   }
                                   else
                                   {
                                       log("GameServer::doConnet> connect retry...");
                                       connect(ip, port);
                                   }
                               });
    
    if ( !_thread.joinable() )
        _thread = boost::thread(boost::bind(&boost::asio::io_service::run, &_io));
}


void GameServer::close()
{
    _io.stop();
    _socket.close();
    _isConnected = false;
    
    if ( _thread.joinable() )
        _thread.join();
}


void GameServer::write(Packet* packet)
{
    _io.post([this, packet]
    {
        bool writeInProgress = !_writeBuf.empty();
        _writeBuf.push_back(packet);
        if (!writeInProgress)
        {
            doWrite();
        }
    });
}


void GameServer::doWrite()
{
    boost::asio::async_write(_socket,
                             boost::asio::buffer(_writeBuf.front()->data(), _writeBuf.front()->length()),
                             [this](boost::system::error_code ec, std::size_t /*length*/)
                             {
                                 if (!ec)
                                 {
                                     Packet* packet = _writeBuf.front();
                                     CC_SAFE_DELETE(packet);
                                     _writeBuf.pop_front();
                                     
                                     if (!_writeBuf.empty())
                                     {
                                         doWrite();
                                     }
                                 }
                                 else if (boost::asio::error::eof == ec || boost::asio::error::connection_reset == ec)
                                 {
                                     close();
                                 }
                             });
}


void GameServer::doReadHeader()
{
    boost::asio::async_read(_socket,
                            boost::asio::buffer(_recvBuf.data(), Packet::HEADER_LENGTH),
                            [this](boost::system::error_code ec, std::size_t /*length*/)
                            {
                                if (!ec && _recvBuf.decode())
                                {
                                    doReadBody();
                                }
                                else if (boost::asio::error::eof == ec || boost::asio::error::connection_reset == ec)
                                {
                                    close();
                                }
                            });
}


void GameServer::doReadBody()
{
    boost::asio::async_read(_socket,
                            boost::asio::buffer(_recvBuf.body(), _recvBuf.bodyLength()),
                            [this](boost::system::error_code ec, std::size_t /*length*/)
                            {
                                if (!ec)
                                {
                                    Packet* packet = new Packet();
                                    std::memcpy(packet->data(), _recvBuf.data(), _recvBuf.length());
                                    enqueue(packet);
                                    
                                    doReadHeader();
                                }
                                else if (boost::asio::error::eof == ec || boost::asio::error::connection_reset == ec)
                                {
                                    close();
                                }
                            });
    
}









