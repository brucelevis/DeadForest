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
    
    class GameManager;
    
    class LogicStream : public MessageNode
    {
        
    public:
        
        LogicStream(GameManager* mgr) : _gameMgr(mgr) {}
        
        virtual ~LogicStream() {}
        
        virtual bool handleMessage(const Telegram& msg) override { return false; }
        
        virtual void processPacket(float dt) {}
        
    protected:
        
        GameManager* _gameMgr;
        
    };
}









