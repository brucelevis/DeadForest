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
#include "EntityPlayer.hpp"
#include "HumanOwnedAnimations.hpp"
#include "Game.hpp"
using namespace cocos2d;
using namespace realtrick::client;


//
// HumanAxeIdleLoop
//
void HumanAxeIdleLoop::enter(EntityPlayer* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanAxeIdleLoop::getInstance());
    human->setVelocity( Vec2::ZERO );
    human->setStateName("idle");
}

void HumanAxeIdleLoop::execute(EntityPlayer* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if ( human->getAnimator()->isQueueEmpty() )
    {
        human->getAnimator()->pushAnimationFrames(&AnimHumanAxeIdleLoop::getInstance());
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK) )
    {
        human->getFSM()->changeState(&HumanAxeAttackReady::getInstance());
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

void HumanAxeIdleLoop::exit(EntityPlayer* human)
{
    human->getAnimator()->clearFrameQueue();
}



//
// HumanAxeMoveLoop
//
void HumanAxeMoveLoop::enter(EntityPlayer* human)
{
    human->getAnimator()->pushFramesAtoB(&AnimHumanAxeMoveLoop::getInstance(), 0, 5);
    human->setRunStats(true);
    human->setStateName("run");
}

void HumanAxeMoveLoop::execute(EntityPlayer* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    int currFrame = human->getAnimator()->getFrameIndex();
    
    if( human->getAnimator()->isQueueEmpty() )
    {
        human->getFSM()->changeState(&HumanAxeIdleLoop::getInstance());
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK) || isMasked(inputMask, HumanBehaviorType::TURN) )
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
            human->getAnimator()->pushFramesAtoB(&AnimHumanAxeMoveLoop::getInstance(), 6, 11);
        }
        else if( currFrame == 11 )
        {
            human->getAnimator()->pushFramesAtoB(&AnimHumanAxeMoveLoop::getInstance(), 0, 5);
        }
    }
}

void HumanAxeMoveLoop::exit(EntityPlayer* human)
{
    human->setRunStats(false);
    human->getAnimator()->clearFrameQueue();
}



//
// HumanAxeOut
//
void HumanAxeOut::enter(EntityPlayer* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanAxeOut::getInstance());
    
    SoundSource s;
    s.fileName = "AxeEnter.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 200.0f;
    human->getGame()->sendMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    human->setStateName("equip weapon");
}

void HumanAxeOut::execute(EntityPlayer* human)
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
        human->getFSM()->changeState(&HumanAxeIdleLoop::getInstance());
    }
}

void HumanAxeOut::exit(EntityPlayer* human)
{
    human->getAnimator()->clearFrameQueue();
}



//
// HumanAxeIn
//
void HumanAxeIn::enter(EntityPlayer* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanAxeIn::getInstance());
    
    SoundSource s;
    s.fileName = "M16A2Enter.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 200.0f;
    human->getGame()->sendMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    human->setStateName("release weapon");
}

void HumanAxeIn::execute(EntityPlayer* human)
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

void HumanAxeIn::exit(EntityPlayer* human)
{
    human->getAnimator()->clearFrameQueue();
}




//
// HumanAxeAttackReady
//
void HumanAxeAttackReady::enter(EntityPlayer* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanAxeAttackReady::getInstance());
    human->setStateName("lifting axe");
}

void HumanAxeAttackReady::execute(EntityPlayer* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if ( human->getAnimator()->isQueueEmpty() )
    {
        if ( isMasked(inputMask, HumanBehaviorType::ATTACK ) )
        {
            human->getFSM()->changeState(&HumanAxeAttackHover::getInstance());
        }
        else
        {
            human->getFSM()->changeState(&HumanAxeAttackRelease::getInstance());
        }
    }
    
    if( isMasked(inputMask, (int)HumanBehaviorType::RUN) ) human->setVelocity( moving * human->getWalkSpeed() );
    else human->setVelocity( Vec2::ZERO );
}

void HumanAxeAttackReady::exit(EntityPlayer* human)
{
    human->getAnimator()->clearFrameQueue();
}


//
// HumanAxeAttackRelease
//
void HumanAxeAttackRelease::enter(EntityPlayer* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanAxeAttackRelease::getInstance());
    human->setStateName("take down axe");
}

void HumanAxeAttackRelease::execute(EntityPlayer* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if ( human->getAnimator()->isQueueEmpty() )
    {
        human->getFSM()->changeState(&HumanAxeIdleLoop::getInstance());
    }
    
    if( isMasked(inputMask, (int)HumanBehaviorType::RUN) ) human->setVelocity( moving * human->getWalkSpeed() );
    else human->setVelocity( Vec2::ZERO );
}

void HumanAxeAttackRelease::exit(EntityPlayer* human)
{
    human->getAnimator()->clearFrameQueue();
}



//
// HumanAxeAttackHover
//
void HumanAxeAttackHover::enter(EntityPlayer* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanAxeAttackHover::getInstance());
    human->setStateName("attack ready");
}

void HumanAxeAttackHover::execute(EntityPlayer* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if ( human->getAnimator()->isQueueEmpty() )
    {
        if ( isMasked(inputMask, HumanBehaviorType::ATTACK ) )
        {
            human->getFSM()->changeState(&HumanAxeAttackHover::getInstance());
        }
    }
    
    if ( !isMasked(inputMask, HumanBehaviorType::ATTACK ) )
    {
        human->getFSM()->changeState(&HumanAxeAttackAction::getInstance());
    }
    
    if( isMasked(inputMask, (int)HumanBehaviorType::RUN) ) human->setVelocity( moving * human->getWalkSpeed() );
    else human->setVelocity( Vec2::ZERO );
}

void HumanAxeAttackHover::exit(EntityPlayer* human)
{
    human->getAnimator()->clearFrameQueue();
}



//
// HumanAxeAttackAction
//
void HumanAxeAttackAction::enter(EntityPlayer* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanAxeAttackAction::getInstance());
    human->setStateName("attack");
    human->getEquipedWeapon()->attack();
}

void HumanAxeAttackAction::execute(EntityPlayer* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if ( human->getAnimator()->isQueueEmpty() )
    {
        human->getFSM()->changeState(&HumanAxeIdleLoop::getInstance());
    }
    
    if( isMasked(inputMask, (int)HumanBehaviorType::RUN) ) human->setVelocity( moving * human->getWalkSpeed() );
    else human->setVelocity( Vec2::ZERO );
}

void HumanAxeAttackAction::exit(EntityPlayer* human)
{
    human->getAnimator()->clearFrameQueue();
}









