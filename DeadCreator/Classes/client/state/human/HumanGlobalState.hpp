//
//  HumanGlobalState.hpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 2. 29..
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
        // HumanGlobalState
        //
        class HumanGlobalState : public State<EntityHuman>, public Singleton<HumanGlobalState>
        {
            
        public:
            
            virtual void enter(EntityHuman* human) override;
            virtual void execute(EntityHuman* human) override;
            virtual void exit(EntityHuman* human) override;
            virtual bool onMessage(EntityHuman* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanGlobalState>;
            HumanGlobalState() = default;
            virtual ~HumanGlobalState() = default;
            
        };
        
    }
}