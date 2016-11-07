//
//  TCPSession.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 5. 3..
//
//

#include "TCPSession.hpp"
#include "Packet.hpp"
#include "cocos2d.h"
using namespace cocos2d;
using namespace realtrick::network;


TCPSession::TCPSession() : _socket(_io), _queue(512), _isConnected(false)
{
}


TCPSession::~TCPSession()
{
    close();
}


void TCPSession::connect(const std::string& ip, const std::string& port)
{
    boost::asio::ip::tcp::resolver resolver(_io);
    auto endpoint = resolver.resolve( { ip, port } );
    boost::asio::async_connect(_socket, endpoint,
                               [this, ip, port] (boost::system::error_code ec, boost::asio::ip::tcp::resolver::iterator)
                               {
                                   if ( !ec )
                                   {
                                       log("TCPSession::doConnet> connected!");
                                       doReadHeader();
                                   }
                                   else
                                   {
                                       log("TCPSession::doConnet> connect retry...");
                                       connect(ip, port);
                                   }
                               });
    
    if ( !_thread.joinable() )
        _thread = boost::thread(boost::bind(&boost::asio::io_service::run, &_io));
}


void TCPSession::close()
{
    _io.stop();
    _socket.close();
    _isConnected = false;
    
    if ( _thread.joinable() )
        _thread.join();
}


void TCPSession::write(Packet* packet)
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


void TCPSession::doWrite()
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
                             });
}


void TCPSession::doReadHeader()
{
    boost::asio::async_read(_socket,
                            boost::asio::buffer(_recvBuf.data(), Packet::HEADER_LENGTH),
                            [this](boost::system::error_code ec, std::size_t /*length*/)
                            {
                                if (!ec && _recvBuf.decode())
                                {
                                    doReadBody();
                                }
                            });
}


void TCPSession::doReadBody()
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
                            });
    
}









