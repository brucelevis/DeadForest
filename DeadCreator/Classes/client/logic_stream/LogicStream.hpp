//
//  LogicStream.hpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 4. 3..
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
        
        class GameManager;
        
        class LogicStream : public MessageNode
        {
            
        public:
            
            explicit LogicStream(GameManager* mgr) : _gameMgr(mgr) {}
            virtual ~LogicStream() = default;
            virtual bool handleMessage(const Telegram& msg) override { return false; }
            virtual void update(float dt) = 0;
            
        protected:
            
            GameManager* _gameMgr;
            
        };
        
    }
}









