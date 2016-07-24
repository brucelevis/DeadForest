//
//  HumanDeadStates.cpp
//  TheDeadForest
//
//  Created by mac on 2016. 2. 29..
//
//

#include "HumanDeadStates.hpp"
#include "MessageDispatcher.hpp"
#include "MessageTypes.hpp"
#include "Telegram.hpp"
#include "EntityHuman.hpp"
#include "GameManager.hpp"
#include "HumanOwnedAnimations.hpp"

namespace realtrick
{
    
    void HumanBackDeadState::enter(EntityHuman* human)
    {
        Dispatch.pushMessage(5.0f, human, human, MessageType::DIE, nullptr);
        human->setDead();
        human->getBodyAnimator()->enableShadow(false);
        human->getBodyAnimator()->pushAnimationFrames(&AnimHumanBackDead::getInstance());
        human->setVelocity( Vec2::ZERO );
    }
    
    
    void HumanBackDeadState::execute(EntityHuman* human)
    {
    }
    
    
    void HumanBackDeadState::exit(EntityHuman* human)
    {
        human->getBodyAnimator()->clearFrameQueue();
    }
    
    
    bool HumanBackDeadState::onMessage(EntityHuman* human, const Telegram& msg)
    {
        if ( msg.msg == MessageType::DIE )
        {
            human->getGameManager()->removeEntity(human->getTag());
            
            return true;
        }
        
        return false;
    }
    
}









