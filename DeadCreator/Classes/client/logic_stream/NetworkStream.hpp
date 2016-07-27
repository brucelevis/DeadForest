//
//  NetworkStream.hpp
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
        
        class NetworkStream : public LogicStream
        {
            
        public:
            
            explicit NetworkStream(GameManager* mgr) : LogicStream(mgr) {}
            virtual ~NetworkStream() = default;
            virtual bool handleMessage(const Telegram& msg) override;
            virtual void processPacket(float dt) override;
            
        };
    }
}