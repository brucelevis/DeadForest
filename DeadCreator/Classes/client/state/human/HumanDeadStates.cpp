//
//  HumanDeadStates.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 2. 29..
//
//

#include "HumanDeadStates.hpp"
#include "MessageDispatcher.hpp"
#include "MessageTypes.hpp"
#include "Telegram.hpp"
#include "EntityPlayer.hpp"
#include "Game.hpp"
#include "HumanOwnedAnimations.hpp"
using namespace realtrick::client;

void HumanBackDeadState::enter(EntityPlayer* human)
{
    human->getGame()->sendMessage(5.0f, human, human, MessageType::DIE, nullptr);
    human->setDead();
    
    human->getAnimator()->pushAnimationFrames(&AnimHumanBackDead::getInstance());
    human->setVelocity( cocos2d::Vec2::ZERO );
    human->setStateName("dead");
}


void HumanBackDeadState::execute(EntityPlayer* human)
{
}


void HumanBackDeadState::exit(EntityPlayer* human)
{
    human->getAnimator()->clearFrameQueue();
}


bool HumanBackDeadState::onMessage(EntityPlayer* human, const Telegram& msg)
{
    if ( msg.msg == MessageType::DIE )
    {
        human->getGame()->removeEntity(human);
        
        return true;
    }
    
    return false;
}









