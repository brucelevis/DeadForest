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
        
        class EntityPlayer;
        class Telegram;
        
        //
        // HumanBackDeadState
        //
        class HumanBackDeadState : public State<EntityPlayer>, public Singleton<HumanBackDeadState>
        {
            
        public:
            
            virtual void enter(EntityPlayer* human) override;
            virtual void execute(EntityPlayer* human) override;
            virtual void exit(EntityPlayer* human) override;
            virtual bool onMessage(EntityPlayer* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanBackDeadState>;
            HumanBackDeadState() = default;
            virtual ~HumanBackDeadState() = default;

        };
        
    }
}









