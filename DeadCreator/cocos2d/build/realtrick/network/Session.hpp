//
//  Session.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 5. 3..
//
//

#pragma once

#ifndef _ENABLE_ATOMIC_ALIGNMENT_FIX
#define _ENABLE_ATOMIC_ALIGNMENT_FIX
#endif

#include <boost/asio.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include <string>
#include <array>
#include <deque>

#include "Packet.hpp"


namespace realtrick
{
    namespace network
    {
        namespace tcp
        {
            
            class Session
            {
                
            public:
                
                Session();
                virtual ~Session();
                
                void write(Packet* packet);
                void close();
                
                void connect(const std::string& ip, const std::string& port);
                bool isConnected() const { return _isConnected; }
                
                void enqueue(Packet* packet) { while (!_queue.push(packet)); }
                void dequeue(Packet*& packet) { while (!_queue.pop(packet)); }
                bool isQueueEmpty() { return _queue.empty(); }
                
            private:
                
                void doReadHeader();
                void doReadBody();
                void doWrite();
                
            private:
                
                boost::thread _thread;
                boost::asio::io_service _io;
                boost::asio::ip::tcp::socket _socket;
                
                Packet _recvBuf;
                
                std::deque<Packet*> _writeBuf;
                boost::lockfree::queue<Packet*> _queue;
                bool _isConnected;
                
            };
            
        }
    }
}









