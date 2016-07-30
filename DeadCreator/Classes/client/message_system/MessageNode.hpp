//
//  MessageNode.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 1. 2..
//
//

#pragma once


namespace realtrick
{
    namespace client
    {
        
        class Telegram;
        
        struct MessageNode
        {

            virtual bool handleMessage(const Telegram& msg) = 0;
            
        };
        
    }
}









