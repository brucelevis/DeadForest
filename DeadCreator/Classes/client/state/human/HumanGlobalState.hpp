//
//  HumanGlobalState.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 2. 29..
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
        // HumanGlobalState
        //
        class HumanGlobalState : public State<EntityPlayer>, public Singleton<HumanGlobalState>
        {
            
        public:
            
            virtual void enter(EntityPlayer* human) override;
            virtual void execute(EntityPlayer* human) override;
            virtual void exit(EntityPlayer* human) override;
            virtual bool onMessage(EntityPlayer* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanGlobalState>;
            HumanGlobalState() = default;
            virtual ~HumanGlobalState() = default;
            
        };
        
    }
}