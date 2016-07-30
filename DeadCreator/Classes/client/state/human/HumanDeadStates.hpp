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
        
        class EntityHuman;
        class Telegram;
        
        //
        // HumanBackDeadState
        //
        class HumanBackDeadState : public State<EntityHuman>, public Singleton<HumanBackDeadState>
        {
            
        public:
            
            virtual void enter(EntityHuman* human) override;
            virtual void execute(EntityHuman* human) override;
            virtual void exit(EntityHuman* human) override;
            virtual bool onMessage(EntityHuman* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanBackDeadState>;
            HumanBackDeadState() = default;
            virtual ~HumanBackDeadState() = default;

        };
        
    }
}









