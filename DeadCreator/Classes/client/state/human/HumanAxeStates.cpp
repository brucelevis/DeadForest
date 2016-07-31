//
//  HumanAxeStates.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 14..
//
//

#include "HumanAxeStates.hpp"
#include "Telegram.hpp"
#include "MessageTypes.hpp"
#include "MessageDispatcher.hpp"
#include "EntityHuman.hpp"
#include "HumanOwnedAnimations.hpp"
#include "Game.hpp"
using namespace cocos2d;
using namespace realtrick::client;

//
// HumanAxeIdleLoop
//
void HumanAxeIdleLoop::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushAnimationFrames(&AnimHumanAxeIdleLoop::getInstance());
    human->setVelocity( Vec2::ZERO );
    human->setStateName("idle");
}

void HumanAxeIdleLoop::execute(EntityHuman* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if(human->getBodyAnimator()->isQueueEmpty())
    {
        human->getBodyAnimator()->pushAnimationFrames(&AnimHumanAxeIdleLoop::getInstance());
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK_ENDED) )
    {
        human->removeInputMask(HumanBehaviorType::ATTACK_ENDED);
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK_BEGAN) )
    {
        human->getFSM()->changeState(&HumanAxeAttack::getInstance());
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::TURN) )
    {
        human->setVelocity( moving * human->getWalkSpeed() );
    }
    
    if ( inputMask == HumanBehaviorType::RUN )
    {
        human->getFSM()->changeState(&HumanAxeMoveLoop::getInstance());
    }
    
    if ( !isMasked(inputMask, HumanBehaviorType::RUN) )
    {
        human->setVelocity( Vec2::ZERO );
    }
}

void HumanAxeIdleLoop::exit(EntityHuman* human)
{
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanAxeIdleLoop::onMessage(EntityHuman* human, const Telegram& msg)
{
    return false;
}


//
// HumanAxeMoveLoop
//
void HumanAxeMoveLoop::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushFramesAtoB(&AnimHumanAxeMoveLoop::getInstance(), 0, 5);
    human->setRunStats(true);
    human->setStateName("run");
}

void HumanAxeMoveLoop::execute(EntityHuman* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    int currFrame = human->getBodyAnimator()->getFrameIndex();
    
    if( human->getBodyAnimator()->isQueueEmpty() )
    {
        human->getFSM()->changeState(&HumanAxeIdleLoop::getInstance());
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK_ENDED) )
    {
        human->removeInputMask(HumanBehaviorType::ATTACK_ENDED);
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK_BEGAN) || isMasked(inputMask, HumanBehaviorType::TURN) )
    {
        if ( currFrame == 5 || currFrame == 11 )
        {
            human->getFSM()->changeState(&HumanAxeIdleLoop::getInstance());
        }
    }
    
    if ( inputMask == HumanBehaviorType::RUN )
    {
        human->setVelocity( moving * human->getRunSpeed() );
        if( moving != Vec2::ZERO ) human->setTargetHeading(moving);
        
        if ( currFrame == 5 )
        {
            human->getBodyAnimator()->pushFramesAtoB(&AnimHumanAxeMoveLoop::getInstance(), 6, 11);
        }
        else if( currFrame == 11 )
        {
            human->getBodyAnimator()->pushFramesAtoB(&AnimHumanAxeMoveLoop::getInstance(), 0, 5);
        }
    }
}

void HumanAxeMoveLoop::exit(EntityHuman* human)
{
    human->setRunStats(false);
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanAxeMoveLoop::onMessage(EntityHuman* human, const Telegram& msg) { return false; }



//
// HumanAxeAttack
//
void HumanAxeAttack::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushOneFrameUnique(&AnimHumanAxeAttack::getInstance(), 0);
    human->setStateName("attack");
}

void HumanAxeAttack::execute(EntityHuman* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    int currFrame = human->getBodyAnimator()->getFrameIndex();
    
    if ( human->getBodyAnimator()->isQueueEmpty() )
    {
        human->getFSM()->changeState(&HumanAxeIdleLoop::getInstance());
    }
    
    if( isMasked(inputMask, (int)HumanBehaviorType::RUN) )
    {
        human->setVelocity( moving * human->getWalkSpeed() );
    }
    else
    {
        human->setVelocity( Vec2::ZERO );
    }
    
    if ( currFrame < 10 )
    {
        if ( isMasked(inputMask, (int)HumanBehaviorType::ATTACK_BEGAN) )
        {
            int nextFrame = currFrame + 1;
            human->getBodyAnimator()->pushOneFrameUnique(&AnimHumanAxeAttack::getInstance(), nextFrame);
        }
    }
    else if ( currFrame == 10 )
    {
        if ( isMasked(inputMask, (int)HumanBehaviorType::ATTACK_BEGAN) )
        {
            human->getBodyAnimator()->enableForceStop(true);
        }
        else if ( isMasked(inputMask, (int)HumanBehaviorType::ATTACK_ENDED) )
        {
            human->removeInputMask(HumanBehaviorType::ATTACK_ENDED);
            human->getBodyAnimator()->enableForceStop(false);
            human->getBodyAnimator()->pushFramesAtoB(&AnimHumanAxeAttack::getInstance(), 11, 17);
            human->getEquipedWeapon()->attack();
        }
    }
}

void HumanAxeAttack::exit(EntityHuman* human)
{
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanAxeAttack::onMessage(EntityHuman* human, const Telegram& msg) { return false; }



//
// HumanAxeOut
//
void HumanAxeOut::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushAnimationFrames(&AnimHumanAxeOut::getInstance());
    
    SoundSource s;
    s.fileName = "AxeEnter.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 200.0f;
    Dispatch.pushMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    human->setStateName("equip weapon");
}

void HumanAxeOut::execute(EntityHuman* human)
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
        human->getFSM()->changeState(&HumanAxeIdleLoop::getInstance());
    }
}

void HumanAxeOut::exit(EntityHuman* human)
{
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanAxeOut::onMessage(EntityHuman* human, const Telegram& msg) { return false; }



//
// HumanAxeIn
//
void HumanAxeIn::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushAnimationFrames(&AnimHumanAxeIn::getInstance());
    
    SoundSource s;
    s.fileName = "M16A2Enter.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 200.0f;
    Dispatch.pushMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    human->setStateName("release weapon");
}

void HumanAxeIn::execute(EntityHuman* human)
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
        if ( human->getEquipedWeapon() == nullptr )
        {
            // 무기가 없으면 주먹 상태로
            human->getFSM()->changeState(&HumanFistOut::getInstance());
        }
        else
        {
            // 있으면 해당무기를 꺼냄.
            human->getEquipedWeapon()->outWeapon();
        }
    }
}

void HumanAxeIn::exit(EntityHuman* human)
{
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanAxeIn::onMessage(EntityHuman* human, const Telegram& msg) { return false; }









