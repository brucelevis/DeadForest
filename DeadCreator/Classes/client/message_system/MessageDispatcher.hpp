//
//  MessageDispatcher.hpp
//  TheDeadForest
//
//  Created by 남준현 on 2015. 11. 12..
//
//

#pragma once

#include <set>
#include <vector>

#include "MessageTypes.hpp"
#include "Singleton.hpp"

namespace realtrick
{
    namespace client
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
            
            void _discharge(const Telegram& msg);
            
        private:
            
            std::set<Telegram>                              _priorityQ;
            
        };
        
    }
}









