//
//  NetworkStream.hpp
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
        
        class NetworkStream : public LogicStream
        {
            
        public:
            
            explicit NetworkStream(Game* game) : LogicStream(game) {}
            virtual ~NetworkStream() = default;
            virtual bool handleMessage(const Telegram& msg) override;
            virtual void update(float dt) override;
            
        };
    }
}