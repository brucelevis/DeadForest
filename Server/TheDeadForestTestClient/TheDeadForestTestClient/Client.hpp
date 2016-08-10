//
//  Client.hpp
//  TheDeadForestTestClient
//
//  Created by ByoungKwon Do on 2016. 8. 10..
//  Copyright © 2016년 ByoungKwon Do. All rights reserved.
//

#pragma once


#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/lockfree/queue.hpp>

#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <map>
#include <queue>

#include "Server.hpp"
#include "GeneratedPackets.hpp"

#define OP_ROOM_TIMER 1
#define OP_GAME_TIMER 2

namespace realtrick
{
    
    class FlatbufferPacket;
    
    class Client
    {
        
    public:
        
        struct eventToken {
            int eventId;
            unsigned int targetTime;
            int eventType;
        };
        
        class mycomp {
            bool reverse;
        public:
            mycomp() {}
            bool operator() (const eventToken lhs, const eventToken rhs) const
            {
                return (lhs.targetTime > rhs.targetTime);
            }
        };
        
        virtual ~Client() = default;
        
        void timerThread();
        
        void run();
        
    private:
        
        void connectServer();
        
        void searchGame();
        
        void cancelGame();
        
        void successLoadGame();
        
        void moveJoystick(bool isTouched, float x, float y);
        
        unsigned int getTickCount();
        
        void enqueueTimer(int id, unsigned int time, int type);
        
        std::priority_queue<eventToken, std::vector<eventToken>, mycomp> _timerQueue;
        
        boost::thread* _timerThread;
        
        int pid = -1;
        
        int rMode = 1;
        
        int rid = 0;
        
        int gid = 0;
        
    };
    
}