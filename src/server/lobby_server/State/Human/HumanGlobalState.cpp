
#include "HumanGlobalState.hpp"
#include "MessageDispatcher.hpp"
#include "MessageTypes.hpp"
#include "Telegram.hpp"
#include "EntityHuman.hpp"
#include "GameManager.hpp"

namespace realtrick
{
    
    void HumanGlobalState::enter(EntityHuman* human)
    {
        
    }
    
    
    void HumanGlobalState::execute(EntityHuman* human)
    {
        
    }
    
    
    void HumanGlobalState::exit(EntityHuman* human)
    {
        
    }
    
    
    bool HumanGlobalState::onMessage(EntityHuman* human, const Telegram& msg)
    {
        if ( msg.msg == MessageType::WEAPON_READY )
        {
            return true;
        }
        else if ( msg.msg  == MessageType::HITTED_BY_GUN )
        {
            return true;
        }
        else if (msg.msg == MessageType::HITTED_BY_AXE )
        {
            return true;
        }
        else if ( msg.msg == MessageType::PLAY_SOUND )
        {
            return true;
        }
        
        return false;
    }
    
}


