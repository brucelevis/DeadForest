//
//  Server.hpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 5. 3..
//
//

#pragma once

#include <deque>

#include <boost/asio.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/thread.hpp>

#include "Singleton.h"
#include "Packet.hpp"


namespace realtrick
{
    
    class Server : public Singleton<Server>
    {
        
        friend class Singleton<Server>;
        
    public:
        
        virtual ~Server() = default;
        
        void connect();
        
        void write(Packet* packet);
        
        void close();
        
        bool isConnected() const { return _isConnected; }
        
        void enqueue(Packet* p) { while(!_queue.push(p)); }
        
        void dequeue(Packet*& p) { while(!_queue.pop(p)); }
        
        bool isQueueEmpty() const { return _queue.empty(); }
        
    private:
        
        Server() : _socket(_io), _queue(512), _isConnected(false), _pid(1)
        {}
        
        Server(const Server& rhs) = delete;
        
        Server& operator=(const Server& rhs) = delete;
        
        void doConnect(boost::asio::ip::tcp::resolver::iterator endpoint);
        
        void doWrite();
        
        void doReadHeader();
        
        void doReadBody();
        
        void enableConnected(bool enable) { _isConnected = enable; }
        
        void closeSocket() { _socket.close(); }
        
    private:
    
        boost::thread* _thread;
        
        boost::asio::io_service _io;
        
        boost::asio::ip::tcp::socket _socket;
        
        Packet _buffer;
        
        boost::lockfree::queue<Packet*> _queue;
        
        std::deque<Packet*> _writeDeque;
        
        std::atomic<bool> _isConnected;
        
        int _pid;
        
    };
    
}







