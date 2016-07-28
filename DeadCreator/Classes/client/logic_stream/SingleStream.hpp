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
    namespace client
    {
        
    class SingleStream : public LogicStream
    {
        
    public:
        
        explicit SingleStream(GameManager* mgr) : LogicStream(mgr) {}
        virtual ~SingleStream() = default;
        virtual bool handleMessage(const Telegram& msg) override;
        
    };
        
    }
}









