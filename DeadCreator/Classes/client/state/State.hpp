//
//  State.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 12..
//
//

#pragma once

namespace realtrick
{
    namespace client
    {
        
        class Telegram;
        class HumanBase;
        
        struct  State
        {
            
            virtual ~State() = default;
            virtual void enter(HumanBase*) = 0;
            virtual void execute(HumanBase*) = 0;
            virtual void exit(HumanBase*) = 0;
            virtual bool onMessage(HumanBase*, const Telegram&)  { return false; }
            
        };
        
    }
}