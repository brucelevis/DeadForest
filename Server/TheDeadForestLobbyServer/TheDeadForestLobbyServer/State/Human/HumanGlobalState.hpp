#pragma once

#include "State.hpp"
#include "Singleton.h"

namespace realtrick
{
    
    class EntityHuman;
    class Telegram;
    
    //
    // HumanGlobalState
    //
    class HumanGlobalState : public State<EntityHuman>, public Singleton<HumanGlobalState>
    {
        
    private:
        
        friend Singleton<HumanGlobalState>;
        HumanGlobalState() = default;
        virtual ~HumanGlobalState() = default;
        
    public:
        
        virtual void enter(EntityHuman* human) override;
        virtual void execute(EntityHuman* human) override;
        virtual void exit(EntityHuman* human) override;
        virtual bool onMessage(EntityHuman* human, const Telegram& msg) override;
        
    };
    
}