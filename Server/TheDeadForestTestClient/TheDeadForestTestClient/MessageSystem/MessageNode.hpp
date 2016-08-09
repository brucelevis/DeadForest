#pragma once

namespace realtrick
{

    class Telegram;
    
    class MessageNode
    {
        
    public:
        
        virtual bool handleMessage(const Telegram& msg) = 0;
        
    };
    
}