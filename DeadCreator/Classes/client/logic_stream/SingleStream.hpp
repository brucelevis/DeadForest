//
//  SingleStream.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 4. 3..
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
            
            explicit SingleStream(Game* game) : LogicStream(game) {}
            virtual ~SingleStream() = default;
            virtual bool handleMessage(const Telegram& msg) override;
            virtual void update(float dt) override {}
            
        };
        
    }
}









