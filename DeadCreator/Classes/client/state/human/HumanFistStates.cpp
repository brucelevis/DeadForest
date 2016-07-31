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
#include "EntityHuman.hpp"
#include "HumanOwnedAnimations.hpp"
#include "Game.hpp"
using namespace cocos2d;
using namespace realtrick::client;

//
// HumanFistIdleLoop
//
void HumanFistIdleLoop::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushAnimationFrames(&AnimHumanFistIdleLoop::getInstance());
    human->setVelocity( Vec2::ZERO );
    human->setStateName("idle");
}

void HumanFistIdleLoop::execute(EntityHuman* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if(human->getBodyAnimator()->isQueueEmpty())
    {
        human->getBodyAnimator()->pushAnimationFrames(&AnimHumanFistIdleLoop::getInstance());
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

void HumanFistIdleLoop::exit(EntityHuman* human)
{
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanFistIdleLoop::onMessage(EntityHuman* human, const Telegram& msg)
{
    return false;
}


//
// HumanFistMoveLoop
//
void HumanFistMoveLoop::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushFramesAtoB(&AnimHumanFistMoveLoop::getInstance(), 0, 5);
    human->setRunStats(true);
    human->setStateName("run");
}

void HumanFistMoveLoop::execute(EntityHuman* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    int currFrame = human->getBodyAnimator()->getFrameIndex();
    
    if( human->getBodyAnimator()->isQueueEmpty() )
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
            human->getBodyAnimator()->pushFramesAtoB(&AnimHumanFistMoveLoop::getInstance(), 6, 11);
        }
        else if( currFrame == 11 )
        {
            human->getBodyAnimator()->pushFramesAtoB(&AnimHumanFistMoveLoop::getInstance(), 0, 5);
        }
    }
}

void HumanFistMoveLoop::exit(EntityHuman* human)
{
    human->setRunStats(false);
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanFistMoveLoop::onMessage(EntityHuman* human, const Telegram& msg) { return false; }



//
// HumanFistAttack
//
void HumanFistAttack::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushOneFrameUnique(&AnimHumanFistAttack::getInstance(), 0);
    human->setStateName("attack");
}

void HumanFistAttack::execute(EntityHuman* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    int currFrame = human->getBodyAnimator()->getFrameIndex();
    
    if ( human->getBodyAnimator()->isQueueEmpty() )
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
            human->getBodyAnimator()->pushOneFrameUnique(&AnimHumanFistAttack::getInstance(), nextFrame);
        }
    }
    else if ( currFrame == 4 )
    {
        if ( isMasked(inputMask, (int)HumanBehaviorType::ATTACK_BEGAN) )
        {
            human->getBodyAnimator()->enableForceStop(true);
        }
        else if ( isMasked(inputMask, (int)HumanBehaviorType::ATTACK_ENDED) )
        {
            human->removeInputMask(HumanBehaviorType::ATTACK_ENDED);
            human->getBodyAnimator()->enableForceStop(false);
            human->getBodyAnimator()->pushFramesAtoB(&AnimHumanFistAttack::getInstance(), 5, 9);
        }
    }
}

void HumanFistAttack::exit(EntityHuman* human)
{
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanFistAttack::onMessage(EntityHuman* human, const Telegram& msg) { return false; }



//
// HumanFistOut
//
void HumanFistOut::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushAnimationFrames(&AnimHumanFistOut::getInstance());
    human->setStateName("release completed");
}

void HumanFistOut::execute(EntityHuman* human)
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
    
    if(human->getBodyAnimator()->isQueueEmpty())
    {
        human->getFSM()->changeState(&HumanFistIdleLoop::getInstance());
    }
}

void HumanFistOut::exit(EntityHuman* human)
{
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanFistOut::onMessage(EntityHuman* human, const Telegram& msg) { return false; }



//
// HumanFistIn
//
void HumanFistIn::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushAnimationFrames(&AnimHumanFistIn::getInstance());
    
    SoundSource s;
    s.fileName = "M16A2Enter.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 200.0f;
    human->getGame()->sendMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    human->setStateName("pick weapon");
}

void HumanFistIn::execute(EntityHuman* human)
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
    
    if(human->getBodyAnimator()->isQueueEmpty())
    {
        human->getEquipedWeapon()->outWeapon();
    }
}

void HumanFistIn::exit(EntityHuman* human)
{
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanFistIn::onMessage(EntityHuman* human, const Telegram& msg) { return false; }









