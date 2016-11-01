//
//  ServerStream.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 8. 11..
//
//

#pragma once

#include "LogicStream.hpp"

namespace realtrick
{
    namespace client
    {
        
        class ServerStream : public LogicStream
        {
            
        public:
            
            explicit ServerStream(Game* game) : LogicStream(game) {}
            virtual ~ServerStream() = default;
            virtual bool handleMessage(const Telegram& msg) override;
            virtual void update(float dt) override;
            
        };
        
    }
}









