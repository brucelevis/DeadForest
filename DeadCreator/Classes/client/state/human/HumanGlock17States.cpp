//
//  HumanGlock17States.cpp
//  TheDeadForest
//
//  Created by 남준현 on 2015. 11. 14..
//
//

#include "HumanGlock17States.hpp"
#include "Telegram.hpp"
#include "MessageTypes.hpp"
#include "MessageDispatcher.hpp"
#include "EntityHuman.hpp"
#include "HumanOwnedAnimations.hpp"
#include "GameManager.hpp"
#include "WeaponBase.hpp"
#include "GameWorld.hpp"
#include "UiLayer.hpp"
#include "Inventory.hpp"
#include "Items.hpp"
#include "WeaponStatus.hpp"
using namespace cocos2d;
using namespace realtrick::client;

//
// HumanGlock17IdleLoop
//
void HumanGlock17IdleLoop::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushAnimationFrames(&AnimHumanGlock17IdleLoop::getInstance());
    human->setVelocity( Vec2::ZERO );
    human->setStateName("idle");
}

void HumanGlock17IdleLoop::execute(EntityHuman* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if(human->getBodyAnimator()->isQueueEmpty())
    {
        human->getBodyAnimator()->pushAnimationFrames(&AnimHumanGlock17IdleLoop::getInstance());
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK_ENDED) )
    {
        human->removeInputMask(HumanBehaviorType::ATTACK_ENDED);
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK_BEGAN) )
    {
        human->setVelocity( moving * human->getWalkSpeed() );
        
        // 공격준비가 되었을때
        if ( human->getEquipedWeapon()->isReadyToAttack() )
        {
            // 1. 남아있는 총알이 있음.
            if ( human->getEquipedWeapon()->getNumOfLeftRounds() > 0 )
            {
                // 공격 상태로 전환
                human->getFSM()->changeState(&HumanGlock17Attack::getInstance());
            }
            // 2. 남아있는 총알이 없음.
            else
            {
                SoundSource s;
                s.fileName = "Glock17Empty.mp3";
                s.position = human->getWorldPosition();
                s.soundRange = 200.0f;
                Dispatch.pushMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
            }
            
            WeaponBase* weapon = human->getEquipedWeapon();
            Dispatch.pushMessage(weapon->getDelay(), human, human, MessageType::WEAPON_READY, reinterpret_cast<void*>(weapon));
            
            human->getEquipedWeapon()->enableReadyToAttack(false);
        }
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::TURN) )
    {
        human->setVelocity( moving * human->getWalkSpeed() );
    }
    
    if ( inputMask == HumanBehaviorType::RUN )
    {
        human->getFSM()->changeState(&HumanGlock17MoveLoop::getInstance());
    }
    
    if ( !isMasked(inputMask, HumanBehaviorType::RUN) )
    {
        human->setVelocity( Vec2::ZERO );
    }
}

void HumanGlock17IdleLoop::exit(EntityHuman* human)
{
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanGlock17IdleLoop::onMessage(EntityHuman* human, const Telegram& msg)
{
    if ( msg.msg == MessageType::RELOAD_WEAPON )
    {
        human->getFSM()->changeState(&HumanGlock17Reload::getInstance());
        return true;
    }
    
    return false;
}



//
// HumanGlock17MoveLoop
//
void HumanGlock17MoveLoop::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushFramesAtoB(&AnimHumanGlock17MoveLoop::getInstance(), 0, 5);
    human->setRunStats(true);
    human->setStateName("run");
}

void HumanGlock17MoveLoop::execute(EntityHuman* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    int currFrame = human->getBodyAnimator()->getFrameIndex();
    
    if( human->getBodyAnimator()->isQueueEmpty() )
    {
        human->getFSM()->changeState(&HumanGlock17IdleLoop::getInstance());
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK_ENDED) )
    {
        human->removeInputMask(HumanBehaviorType::ATTACK_ENDED);
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK_BEGAN) || isMasked(inputMask, HumanBehaviorType::TURN) )
    {
        if ( currFrame == 5 || currFrame == 11 )
        {
            human->getFSM()->changeState(&HumanGlock17IdleLoop::getInstance());
        }
    }
    
    if ( inputMask == HumanBehaviorType::RUN )
    {
        human->setVelocity( moving * human->getRunSpeed() );
        if( moving != Vec2::ZERO ) human->setTargetHeading(moving);
        
        if ( currFrame == 5 )
        {
            human->getBodyAnimator()->pushFramesAtoB(&AnimHumanGlock17MoveLoop::getInstance(), 6, 11);
        }
        else if( currFrame == 11 )
        {
            human->getBodyAnimator()->pushFramesAtoB(&AnimHumanGlock17MoveLoop::getInstance(), 0, 5);
        }
    }
}

void HumanGlock17MoveLoop::exit(EntityHuman* human)
{
    human->setRunStats(false);
    human->getBodyAnimator()->clearFrameQueue();    }

bool HumanGlock17MoveLoop::onMessage(EntityHuman* human, const Telegram& msg)
{
    if ( msg.msg == MessageType::RELOAD_WEAPON )
    {
        human->getFSM()->changeState(&HumanGlock17Reload::getInstance());
        
        return true;
    }
    return false;
}



//
// HumanGlock17Attack
//
void HumanGlock17Attack::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushAnimationFrames(&AnimHumanGlock17Attack::getInstance());
    
    SoundSource s;
    s.fileName = "Glock17Fire" + _to_string(random(0, 2)) + ".mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 2000.0f;
    Dispatch.pushMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    human->getEquipedWeapon()->attack();
    human->getEquipedWeapon()->setNumOfLeftRounds( human->getEquipedWeapon()->getNumOfLeftRounds() - 1);
    human->getWeaponStatus()->setWeaponStatus(human->getEquipedWeapon());
    
    human->setStateName("attack");
}

void HumanGlock17Attack::execute(EntityHuman* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if( isMasked(inputMask, HumanBehaviorType::RUN) )
    {
        human->setVelocity( moving * human->getWalkSpeed() );
    }
    else
    {
        human->setVelocity( Vec2::ZERO );
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK_ENDED) )
    {
        human->removeInputMask(HumanBehaviorType::ATTACK_ENDED);
    }
    
    if ( human->getBodyAnimator()->isQueueEmpty() )
    {
        human->getFSM()->changeState(&HumanGlock17IdleLoop::getInstance());
    }
}

void HumanGlock17Attack::exit(EntityHuman* human)
{
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanGlock17Attack::onMessage(EntityHuman* human, const Telegram& msg) { return false; }


//
// HumanGlock17Reload
//
void HumanGlock17Reload::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushAnimationFrames(&AnimHumanGlock17Reload::getInstance());
    
    SoundSource s;
    s.fileName = "Glock17Reload.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 400.0f;
    Dispatch.pushMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    human->setStateName("reload");
}

void HumanGlock17Reload::execute(EntityHuman* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if( isMasked(inputMask, HumanBehaviorType::RUN) )
    {
        human->setVelocity( moving * human->getWalkSpeed() );
    }
    else
    {
        human->setVelocity( Vec2::ZERO );
    }
    
    if ( human->getBodyAnimator()->isQueueEmpty() )
    {
        human->getFSM()->changeState(&HumanGlock17IdleLoop::getInstance());
    }
}

void HumanGlock17Reload::exit(EntityHuman* human)
{
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanGlock17Reload::onMessage(EntityHuman* human, const Telegram& msg) { return false; }



//
// HumanGlock17Out
//
void HumanGlock17Out::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushAnimationFrames(&AnimHumanGlock17Out::getInstance());
    
    SoundSource s;
    s.fileName = "GunEnter.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 200.0f;
    Dispatch.pushMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    human->setStateName("equip weapon");
}

void HumanGlock17Out::execute(EntityHuman* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if( isMasked(inputMask, HumanBehaviorType::RUN) )
    {
        human->setVelocity( moving * human->getWalkSpeed() );
    }
    else
    {
        human->setVelocity( Vec2::ZERO );
    }
    
    if(human->getBodyAnimator()->isQueueEmpty())
    {
        human->getFSM()->changeState(&HumanGlock17IdleLoop::getInstance());
    }
}

void HumanGlock17Out::exit(EntityHuman* human)
{
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanGlock17Out::onMessage(EntityHuman* human, const Telegram& msg) { return false; }



//
// HumanGlock17In
//
void HumanGlock17In::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushAnimationFrames(&AnimHumanGlock17In::getInstance());
    
    SoundSource s;
    s.fileName = "M16A2Enter.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 200.0f;
    Dispatch.pushMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    human->setStateName("release weapon");
}

void HumanGlock17In::execute(EntityHuman* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if( isMasked(inputMask, HumanBehaviorType::RUN) )
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

void HumanGlock17In::exit(EntityHuman* human)
{
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanGlock17In::onMessage(EntityHuman* human, const Telegram& msg) { return false; }









