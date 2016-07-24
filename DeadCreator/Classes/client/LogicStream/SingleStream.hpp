//
//  SingleStream.hpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 4. 3..
//
//

#pragma once

#include "LogicStream.hpp"

namespace realtrick
{
    class SingleStream : public LogicStream
    {
        
    public:
        
        SingleStream(GameManager* mgr) : LogicStream(mgr) {}
        
        virtual ~SingleStream() {}
        
        virtual bool handleMessage(const Telegram& msg) override;
        
    };
}









