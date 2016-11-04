//
//  NetworkWriter.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 11. 1..
//
//

#pragma once


#include <vector>
#include <memory>
#include <array>
#include <deque>

#ifndef _ENABLE_ATOMIC_ALIGNMENT_FIX
#define _ENABLE_ATOMIC_ALIGNMENT_FIX
#endif

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "Packet.hpp"

namespace realtrick
{
    namespace profiler
    {
        
        class Session : public std::enable_shared_from_this<Session>
        {
            
        public:
            
            explicit Session(boost::asio::ip::tcp::socket socket) :
            _socket(std::move(socket))
            {}
            virtual ~Session() = default;
            
            void start()
            {
                doReadBody();
            }
            
            void write(Packet* packet)
            {
                bool isEmpty = _writeBuffer.empty();
                _writeBuffer.push_back(packet);
                if ( isEmpty )
                {
                    doWrite();
                }
            }
            
        private:
            
            void doReadHeader()
            {
                auto self(shared_from_this());
                async_read(_socket, boost::asio::buffer(_recvBuffer.data(), Packet::HEADER_LENGTH),
                           [this, self] (const boost::system::error_code& ec, size_t length)
                           {
                               if ( !ec && _recvBuffer.decode() )
                               {
                                   doReadBody();
                               }
                           });
                
            }
            
            void doReadBody()
            {
                auto self(shared_from_this());
                async_read(_socket, boost::asio::buffer(_recvBuffer.body(), _recvBuffer.bodyLength()),
                           [this, self] (const boost::system::error_code& ec, size_t length)
                           {
                               if ( !ec && _recvBuffer.decode() )
                               {
                                   PacketType type = _recvBuffer.type();
                                   if ( type == PacketType::LOGIN )
                                   {}
                                   
                                   doReadHeader();
                               }
                           });
            }
            
            void doWrite()
            {
                auto self(shared_from_this());
                async_write(_socket, boost::asio::buffer(_writeBuffer.front()->data(), _writeBuffer.front()->length()),
                            [this, self](const boost::system::error_code& ec, size_t length)
                            {
                                Packet* packet = _writeBuffer.front();
                                delete packet;
                                packet = nullptr;
                                
                                _writeBuffer.pop_front();
                                if ( !_writeBuffer.empty() )
                                {
                                    doWrite();
                                }
                            });
            }
            
        private:
            
            boost::asio::ip::tcp::socket _socket;
            Packet _recvBuffer;
            std::deque<Packet*> _writeBuffer;
            
        };
        
        
        
        class NetworkWriter
        {
            
        public:
            
            NetworkWriter() :
            _socket(_io),
            _acceptor(_io, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 4242))
            {
                doAccept();
                
                _networkThread = new boost::thread(boost::bind(&boost::asio::io_service::run, &_io));
            }
            virtual ~NetworkWriter()
            {
                _io.stop();
                _socket.close();
                
                
                _networkThread->join();
                delete _networkThread;
                _networkThread = nullptr;
            }
            
        private:
            
            void doAccept()
            {
                _acceptor.async_accept(_socket, [this](const boost::system::error_code& ec) {
                    
                    if ( !ec )
                    {
                        auto session = std::make_shared<Session>(std::move(_socket));
                        session->start();
                        _sessions.push_back(session);
                    }
                    
                    doAccept();
                    
                });
            }
            
        private:
            
            boost::thread* _networkThread;
            
            boost::asio::io_service _io;
            boost::asio::ip::tcp::socket _socket;
            boost::asio::ip::tcp::acceptor _acceptor;
            
            std::vector<std::weak_ptr<Session>> _sessions;
            
        };
        
    }
}









