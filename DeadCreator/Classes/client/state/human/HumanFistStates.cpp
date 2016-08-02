//
//  HumanFistStates.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 14..
//
//

#include "HumanFistStates.hpp"
#include "Telegram.hpp"
#include "MessageTypes.hpp"
#include "MessageDispatcher.hpp"
#include "EntityPlayer.hpp"
#include "HumanOwnedAnimations.hpp"
#include "Game.hpp"
using namespace cocos2d;
using namespace realtrick::client;

//
// HumanFistIdleLoop
//
void HumanFistIdleLoop::enter(EntityPlayer* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanFistIdleLoop::getInstance());
    human->setVelocity( Vec2::ZERO );
    human->setStateName("idle");
}

void HumanFistIdleLoop::execute(EntityPlayer* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if(human->getAnimator()->isQueueEmpty())
    {
        human->getAnimator()->pushAnimationFrames(&AnimHumanFistIdleLoop::getInstance());
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK_ENDED) )
    {
        human->removeInputMask(HumanBehaviorType::ATTACK_ENDED);
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK_BEGAN) )
    {
        human->getFSM()->changeState(&HumanFistAttack::getInstance());
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::TURN) )
    {
        human->setVelocity( moving * human->getWalkSpeed() );
    }
    
    if ( inputMask == HumanBehaviorType::RUN )
    {
        human->getFSM()->changeState(&HumanFistMoveLoop::getInstance());
    }
    
    if ( !isMasked(inputMask, HumanBehaviorType::RUN) )
    {
        human->setVelocity( Vec2::ZERO );
    }
}

void HumanFistIdleLoop::exit(EntityPlayer* human)
{
    human->getAnimator()->clearFrameQueue();
}

bool HumanFistIdleLoop::onMessage(EntityPlayer* human, const Telegram& msg)
{
    return false;
}


//
// HumanFistMoveLoop
//
void HumanFistMoveLoop::enter(EntityPlayer* human)
{
    human->getAnimator()->pushFramesAtoB(&AnimHumanFistMoveLoop::getInstance(), 0, 5);
    human->setRunStats(true);
    human->setStateName("run");
}

void HumanFistMoveLoop::execute(EntityPlayer* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    int currFrame = human->getAnimator()->getFrameIndex();
    
    if( human->getAnimator()->isQueueEmpty() )
    {
        human->getFSM()->changeState(&HumanFistIdleLoop::getInstance());
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK_ENDED) )
    {
        human->removeInputMask(HumanBehaviorType::ATTACK_ENDED);
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK_BEGAN) || isMasked(inputMask, HumanBehaviorType::TURN) )
    {
        if ( currFrame == 5 || currFrame == 11 )
        {
            human->getFSM()->changeState(&HumanFistIdleLoop::getInstance());
        }
    }
    
    if ( inputMask == HumanBehaviorType::RUN )
    {
        human->setVelocity( moving * human->getRunSpeed() );
        if( moving != Vec2::ZERO ) human->setTargetHeading(moving);
        
        if ( currFrame == 5 )
        {
            human->getAnimator()->pushFramesAtoB(&AnimHumanFistMoveLoop::getInstance(), 6, 11);
        }
        else if( currFrame == 11 )
        {
            human->getAnimator()->pushFramesAtoB(&AnimHumanFistMoveLoop::getInstance(), 0, 5);
        }
    }
}

void HumanFistMoveLoop::exit(EntityPlayer* human)
{
    human->setRunStats(false);
    human->getAnimator()->clearFrameQueue();
}

bool HumanFistMoveLoop::onMessage(EntityPlayer* human, const Telegram& msg) { return false; }



//
// HumanFistAttack
//
void HumanFistAttack::enter(EntityPlayer* human)
{
    human->getAnimator()->pushOneFrameUnique(&AnimHumanFistAttack::getInstance(), 0);
    human->setStateName("attack");
}

void HumanFistAttack::execute(EntityPlayer* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    int currFrame = human->getAnimator()->getFrameIndex();
    
    if ( human->getAnimator()->isQueueEmpty() )
    {
        human->getFSM()->changeState(&HumanFistIdleLoop::getInstance());
    }
    
    if( isMasked(inputMask, (int)HumanBehaviorType::RUN) )
    {
        human->setVelocity( moving * human->getWalkSpeed() );
    }
    else
    {
        human->setVelocity( Vec2::ZERO );
    }
    
    if ( currFrame < 4 )
    {
        if ( isMasked(inputMask, (int)HumanBehaviorType::ATTACK_BEGAN) )
        {
            int nextFrame = currFrame + 1;
            human->getAnimator()->pushOneFrameUnique(&AnimHumanFistAttack::getInstance(), nextFrame);
        }
    }
    else if ( currFrame == 4 )
    {
        if ( isMasked(inputMask, (int)HumanBehaviorType::ATTACK_BEGAN) )
        {
            human->getAnimator()->enableForceStop(true);
        }
        else if ( isMasked(inputMask, (int)HumanBehaviorType::ATTACK_ENDED) )
        {
            human->removeInputMask(HumanBehaviorType::ATTACK_ENDED);
            human->getAnimator()->enableForceStop(false);
            human->getAnimator()->pushFramesAtoB(&AnimHumanFistAttack::getInstance(), 5, 9);
        }
    }
}

void HumanFistAttack::exit(EntityPlayer* human)
{
    human->getAnimator()->clearFrameQueue();
}

bool HumanFistAttack::onMessage(EntityPlayer* human, const Telegram& msg) { return false; }



//
// HumanFistOut
//
void HumanFistOut::enter(EntityPlayer* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanFistOut::getInstance());
    human->setStateName("release completed");
}

void HumanFistOut::execute(EntityPlayer* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if( isMasked(inputMask, (int)HumanBehaviorType::RUN) )
    {
        human->setVelocity( moving * human->getWalkSpeed() );
    }
    else
    {
        human->setVelocity( Vec2::ZERO );
    }
    
    if(human->getAnimator()->isQueueEmpty())
    {
        human->getFSM()->changeState(&HumanFistIdleLoop::getInstance());
    }
}

void HumanFistOut::exit(EntityPlayer* human)
{
    human->getAnimator()->clearFrameQueue();
}

bool HumanFistOut::onMessage(EntityPlayer* human, const Telegram& msg) { return false; }



//
// HumanFistIn
//
void HumanFistIn::enter(EntityPlayer* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanFistIn::getInstance());
    
    SoundSource s;
    s.fileName = "M16A2Enter.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 200.0f;
    human->getGame()->sendMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    human->setStateName("pick weapon");
}

void HumanFistIn::execute(EntityPlayer* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if( isMasked(inputMask, (int)HumanBehaviorType::RUN) )
    {
        human->setVelocity( moving * human->getWalkSpeed() );
    }
    else
    {
        human->setVelocity( Vec2::ZERO );
    }
    
    if(human->getAnimator()->isQueueEmpty())
    {
        human->getEquipedWeapon()->outWeapon();
    }
}

void HumanFistIn::exit(EntityPlayer* human)
{
    human->getAnimator()->clearFrameQueue();
}

bool HumanFistIn::onMessage(EntityPlayer* human, const Telegram& msg) { return false; }









