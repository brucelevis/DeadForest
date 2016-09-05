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
#include "HumanBase.hpp"
#include "HumanOwnedAnimations.hpp"
#include "Game.hpp"
using namespace cocos2d;
using namespace realtrick::client;

//
// HumanFistIdleLoop
//
void HumanFistIdleLoop::enter(HumanBase* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanFistIdleLoop::getInstance());
    human->setVelocity( Vec2::ZERO );
    human->setStateName("idle");
}

void HumanFistIdleLoop::execute(HumanBase* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if(human->getAnimator()->isQueueEmpty())
    {
        human->getAnimator()->pushAnimationFrames(&AnimHumanFistIdleLoop::getInstance());
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK) )
    {
        human->getFSM()->changeState(&HumanFistAttackReady::getInstance());
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::TURN) )
    {
        human->setVelocity( moving * human->getWalkSpeed() );
    }
    
    if ( inputMask == HumanBehaviorType::MOVE )
    {
        human->getFSM()->changeState(&HumanFistMoveLoop::getInstance());
    }
    
    if ( !isMasked(inputMask, HumanBehaviorType::MOVE) )
    {
        human->setVelocity( Vec2::ZERO );
    }
}

void HumanFistIdleLoop::exit(HumanBase* human)
{
    human->getAnimator()->clearFrameQueue();
}


//
// HumanFistMoveLoop
//
void HumanFistMoveLoop::enter(HumanBase* human)
{
    human->getAnimator()->pushFramesAtoB(&AnimHumanFistMoveLoop::getInstance(), 0, 5);
    human->setRunStats(true);
    human->setStateName("run");
}

void HumanFistMoveLoop::execute(HumanBase* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    int currFrame = human->getAnimator()->getFrameIndex();
    
    if( human->getAnimator()->isQueueEmpty() )
    {
        human->getFSM()->changeState(&HumanFistIdleLoop::getInstance());
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK) || isMasked(inputMask, HumanBehaviorType::TURN) )
    {
        if ( currFrame == 5 || currFrame == 11 )
        {
            human->getFSM()->changeState(&HumanFistIdleLoop::getInstance());
        }
    }
    
    if ( inputMask == HumanBehaviorType::MOVE )
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

void HumanFistMoveLoop::exit(HumanBase* human)
{
    human->setRunStats(false);
    human->getAnimator()->clearFrameQueue();
}


//
// HumanFistOut
//
void HumanFistOut::enter(HumanBase* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanFistOut::getInstance());
    human->setStateName("release completed");
}

void HumanFistOut::execute(HumanBase* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if( isMasked(inputMask, (int)HumanBehaviorType::MOVE) )
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

void HumanFistOut::exit(HumanBase* human)
{
    human->getAnimator()->clearFrameQueue();
}


//
// HumanFistIn
//
void HumanFistIn::enter(HumanBase* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanFistIn::getInstance());
    
    SoundSource s;
    s.fileName = "M16A2Enter.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 200.0f;
    human->getGame()->sendMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    human->setStateName("pick weapon");
}

void HumanFistIn::execute(HumanBase* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if( isMasked(inputMask, (int)HumanBehaviorType::MOVE) )
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

void HumanFistIn::exit(HumanBase* human)
{
    human->getAnimator()->clearFrameQueue();
}


//
// HumanFistAttackReady
//
void HumanFistAttackReady::enter(HumanBase* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanFistAttackReady::getInstance());
    human->setStateName("raise right hand");
}

void HumanFistAttackReady::execute(HumanBase* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if ( human->getAnimator()->isQueueEmpty() )
    {
        if ( isMasked(inputMask, HumanBehaviorType::ATTACK ) )
        {
            human->getFSM()->changeState(&HumanFistAttackHover::getInstance());
        }
        else
        {
            human->getFSM()->changeState(&HumanFistAttackRelease::getInstance());
        }
    }
    
    if( isMasked(inputMask, (int)HumanBehaviorType::MOVE) ) human->setVelocity( moving * human->getWalkSpeed() );
    else human->setVelocity( Vec2::ZERO );
}

void HumanFistAttackReady::exit(HumanBase* human)
{
    human->getAnimator()->clearFrameQueue();
}


//
// HumanFistAttackRelease
//
void HumanFistAttackRelease::enter(HumanBase* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanFistAttackRelease::getInstance());
    human->setStateName("take down right hand");
}

void HumanFistAttackRelease::execute(HumanBase* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if ( human->getAnimator()->isQueueEmpty() )
    {
        human->getFSM()->changeState(&HumanFistIdleLoop::getInstance());
    }
    
    if( isMasked(inputMask, (int)HumanBehaviorType::MOVE) ) human->setVelocity( moving * human->getWalkSpeed() );
    else human->setVelocity( Vec2::ZERO );
}

void HumanFistAttackRelease::exit(HumanBase* human)
{
    human->getAnimator()->clearFrameQueue();
}


//
// HumanFistAttackHover
//
void HumanFistAttackHover::enter(HumanBase* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanFistAttackHover::getInstance());
    human->setStateName("attack ready");
}

void HumanFistAttackHover::execute(HumanBase* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if ( human->getAnimator()->isQueueEmpty() )
    {
        if ( isMasked(inputMask, HumanBehaviorType::ATTACK ) )
        {
            human->getFSM()->changeState(&HumanFistAttackHover::getInstance());
        }
    }
    
    if ( !isMasked(inputMask, HumanBehaviorType::ATTACK ) )
    {
        human->getFSM()->changeState(&HumanFistAttackAction::getInstance());
    }
    
    if( isMasked(inputMask, (int)HumanBehaviorType::MOVE) ) human->setVelocity( moving * human->getWalkSpeed() );
    else human->setVelocity( Vec2::ZERO );
}

void HumanFistAttackHover::exit(HumanBase* human)
{
    human->getAnimator()->clearFrameQueue();
}


//
// HumanFistAttackAction
//
void HumanFistAttackAction::enter(HumanBase* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanFistAttackAction::getInstance());
    human->setStateName("attack");
}

void HumanFistAttackAction::execute(HumanBase* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if ( human->getAnimator()->isQueueEmpty() )
    {
        human->getFSM()->changeState(&HumanFistIdleLoop::getInstance());
    }
    
    if( isMasked(inputMask, (int)HumanBehaviorType::MOVE) ) human->setVelocity( moving * human->getWalkSpeed() );
    else human->setVelocity( Vec2::ZERO );

}

void HumanFistAttackAction::exit(HumanBase* human)
{
    human->getAnimator()->clearFrameQueue();
}









