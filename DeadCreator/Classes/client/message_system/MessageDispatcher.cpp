//
//  MessageDispatcher.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 12..
//
//

#include <chrono>

#include "cocos2d.h"

#include "MessageDispatcher.hpp"
#include "Game.hpp"
using namespace cocos2d;
using namespace realtrick::client;


void MessageDispatcher::discharge(const Telegram& msg)
{
    bool ret = false;
    if ( msg.receiver )
    {
        ret = msg.receiver->handleMessage(msg);
    }
    
    if( ret == false)
    {
        cocos2d::log("<MessageDispatcher:discharge> Message is not handled. MessageType: [%d]", static_cast<int>(msg.msg));
    }
}


void MessageDispatcher::pushMessage(double delaySeconds,
                                    MessageNode* receiver,
                                    MessageNode* sender,
                                    MessageType type,
                                    void* extraInfo)
{
    Telegram tel(delaySeconds, receiver, sender, type, extraInfo);
    if(tel.dispatchTime <= std::chrono::duration<double>::zero())
    {
        discharge(tel);
    }
    else
    {
        std::chrono::duration<double> curr = std::chrono::system_clock::now().time_since_epoch();
        tel.dispatchTime += curr;
        _priorityQ.insert(tel);
    }
}


void MessageDispatcher::dispatchDelayedMessages()
{
    std::chrono::duration<double> curr = std::chrono::system_clock::now().time_since_epoch();
    
    while (!_priorityQ.empty() &&
           (_priorityQ.begin()->dispatchTime < curr) &&
           (_priorityQ.begin()->dispatchTime >= std::chrono::duration<double>::zero()))
    {
        const Telegram& telegram = *_priorityQ.begin();
        discharge(telegram);
        _priorityQ.erase(_priorityQ.begin());
    }
}


void MessageDispatcher::clearQueue()
{
    _priorityQ.clear();
}















