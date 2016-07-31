//
//  MessageDispatcher.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 12..
//
//

#pragma once

#include <set>
#include <vector>

#include "MessageTypes.hpp"
#include "Telegram.hpp"
#include "MessageNode.hpp"

namespace realtrick
{
    namespace client
    {
        
        class MessageDispatcher
        {
            
        public:
            
            MessageDispatcher() = default;
            virtual ~MessageDispatcher() { clearQueue(); }
            
            void pushMessage(double delaySeconds, MessageNode* receiver, MessageNode* sender, MessageType type, void* extraInfo);
            void dispatchDelayedMessages();
            void clearQueue();
            
        private:
            
            void discharge(const Telegram& msg);
            
        private:
            
            std::set<Telegram> _priorityQ;
            
        };
        
    }
}









