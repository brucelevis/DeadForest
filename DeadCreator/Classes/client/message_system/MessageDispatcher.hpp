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
        
        class Game;
        
        class MessageDispatcher : public cocos2d::Ref
        {
            
        public:
            
            explicit MessageDispatcher(Game* game) : _game(game)
            {}
            virtual ~MessageDispatcher() { clearQueue(); }
            
            static MessageDispatcher* create(Game* game)
            {
                auto ret = new (std::nothrow) MessageDispatcher(game);
                if ( ret && ret->init() )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool init() { return true; }
            
            
            void pushMessage(double delaySeconds, MessageNode* receiver, MessageNode* sender, MessageType type, void* extraInfo);
            void dispatchDelayedMessages();
            void clearQueue();
            
        private:
            
            void discharge(const Telegram& msg);
            
        private:
            
            Game* _game;
            std::set<Telegram> _priorityQ;
            
        };
        
    }
}









