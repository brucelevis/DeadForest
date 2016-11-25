//
//  HumanDeadStates.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 5. 15..
//
//

#pragma once

#include "State.hpp"
#include "Singleton.hpp"

namespace realtrick
{
    namespace client
    {
        
        class HumanBase;
        class Telegram;
        
        //
        // HumanBackDeadState
        //
        class HumanBackDeadState : public State, public Singleton<HumanBackDeadState>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            virtual bool onMessage(HumanBase* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanBackDeadState>;
            HumanBackDeadState() = default;
            virtual ~HumanBackDeadState() = default;

        };
        
    }
}









