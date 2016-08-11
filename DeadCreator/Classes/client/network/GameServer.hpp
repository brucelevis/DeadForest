//
//  GameServer.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 5. 3..
//
//

#pragma once

#include <boost/asio.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/thread.hpp>

#include <string>
#include <array>

#include "Singleton.hpp"
#include "Packet.hpp"

#include "GeneratedPackets.hpp"


namespace realtrick
{
    namespace client
    {
        
        class GameServer : public realtrick::Singleton<GameServer>
        {
            
            friend class Singleton<GameServer>;
            
        public:
            
            virtual ~GameServer();
            
            void write(Packet* packet);
            void close();
            
            void connect(const std::string& ip, const std::string& port);
            bool isConnected() const { return _isConnected; }
            
            void enqueue(Packet* packet) { while (!_queue.push(packet)); }
            void dequeue(Packet*& packet) { while (!_queue.pop(packet)); }
            bool isQueueEmpty() { return _queue.empty(); }
            
        private:
            
            GameServer();
            
            void doReadHeader();
            void doReadBody();
            void doWrite();
            
        private:
            
            boost::thread* _thread;
            boost::asio::io_service _io;
            boost::asio::ip::tcp::socket _socket;
            
            Packet _recvBuf;
            
            std::deque<Packet*> _writeBuf;
            boost::lockfree::queue<Packet*> _queue;
            bool _isConnected;
            
        };
        
    }
}









