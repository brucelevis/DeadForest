//
//  LogicStream.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 4. 3..
//
//

#pragma once

#include <cocos2d.h>

#include "MessageNode.hpp"
#include "MessageDispatcher.hpp"
#include "Telegram.hpp"
#include "MessageTypes.hpp"

namespace realtrick
{
    namespace client
    {
        
        class Game;
        
        class LogicStream : public MessageNode
        {
            
        public:
            
            explicit LogicStream(Game* game) : _game(game) {}
            virtual ~LogicStream() = default;
            virtual bool handleMessage(const Telegram& msg) override { return false; }
            virtual void update(float dt) = 0;
            
        protected:
            
            Game* _game;
            
        };
        
    }
}









