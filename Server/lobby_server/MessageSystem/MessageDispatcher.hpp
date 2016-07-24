#pragma once

#include <set>
#include <vector>
#include <iostream>

#include "MessageTypes.hpp"
#include "Singleton.h"

namespace realtrick
{
    class Telegram;
    class MessageNode;
    
#define Dispatch MessageDispatcher::getInstance()
    
    class MessageDispatcher : public Singleton<MessageDispatcher>
    {
        
    public:
        
        void pushMessage(double delaySeconds, MessageNode* receiver, MessageNode* sender, MessageType type, void* extraInfo);
        
        void dispatchDelayedMessages();
        
        void clearQueue();
        
    private:
        
        std::set<Telegram>                              _priorityQ;
        
    private:
        
        void _discharge(const Telegram& msg);
        
    };
    
}





