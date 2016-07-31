//
//  HumanM16A2States.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 14..
//
//

#include "HumanM16A2States.hpp"
#include "Telegram.hpp"
#include "MessageTypes.hpp"
#include "MessageDispatcher.hpp"
#include "EntityHuman.hpp"
#include "HumanOwnedAnimations.hpp"
#include "Game.hpp"
#include "WeaponBase.hpp"
#include "UiLayer.hpp"
#include "Inventory.hpp"
#include "Items.hpp"
#include "WeaponStatus.hpp"
using namespace cocos2d;
using namespace realtrick::client;

//
// HumanM16A2IdleLoop
//
void HumanM16A2IdleLoop::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushAnimationFrames(&AnimHumanM16A2IdleLoop::getInstance());
    human->setVelocity( Vec2::ZERO );
    human->setStateName("idle");
}

void HumanM16A2IdleLoop::execute(EntityHuman* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if(human->getBodyAnimator()->isQueueEmpty())
    {
        human->getBodyAnimator()->pushAnimationFrames(&AnimHumanM16A2IdleLoop::getInstance());
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
                human->getFSM()->changeState(&HumanM16A2Attack::getInstance());
            }
            // 2. 남아있는 총알이 없음.
            else
            {
                SoundSource s;
                s.fileName = "Glock17Empty.mp3";
                s.position = human->getWorldPosition();
                s.soundRange = 200.0f;
                human->getGame()->sendMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
            }
            
            WeaponBase* weapon = human->getEquipedWeapon();
            human->getGame()->sendMessage(weapon->getDelay(), human, human, MessageType::WEAPON_READY, reinterpret_cast<void*>(weapon));
            
            human->getEquipedWeapon()->enableReadyToAttack(false);
        }
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::TURN) )
    {
        human->setVelocity( moving * human->getWalkSpeed() );
    }
    
    if ( inputMask == HumanBehaviorType::RUN )
    {
        human->getFSM()->changeState(&HumanM16A2MoveLoop::getInstance());
    }
    
    if ( !isMasked(inputMask, HumanBehaviorType::RUN) )
    {
        human->setVelocity( Vec2::ZERO );
    }
}

void HumanM16A2IdleLoop::exit(EntityHuman* human)
{
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanM16A2IdleLoop::onMessage(EntityHuman* human, const Telegram& msg)
{
    if ( msg.msg == MessageType::RELOAD_WEAPON )
    {
        human->getFSM()->changeState(&HumanM16A2Reload::getInstance());
        return true;
    }
    
    return false;
}



//
// HumanM16A2MoveLoop
//
void HumanM16A2MoveLoop::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushFramesAtoB(&AnimHumanM16A2MoveLoop::getInstance(), 0, 5);
    human->setRunStats(true);
    human->setStateName("run");
}

void HumanM16A2MoveLoop::execute(EntityHuman* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    int currFrame = human->getBodyAnimator()->getFrameIndex();
    
    if( human->getBodyAnimator()->isQueueEmpty() )
    {
        human->getFSM()->changeState(&HumanM16A2IdleLoop::getInstance());
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK_ENDED) )
    {
        human->removeInputMask(HumanBehaviorType::ATTACK_ENDED);
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK_BEGAN) || isMasked(inputMask, HumanBehaviorType::TURN) )
    {
        if ( currFrame == 5 || currFrame == 11 )
        {
            human->getFSM()->changeState(&HumanM16A2IdleLoop::getInstance());
        }
    }
    
    if ( inputMask == HumanBehaviorType::RUN )
    {
        human->setVelocity( moving * human->getRunSpeed() );
        if( moving != Vec2::ZERO ) human->setTargetHeading(moving);
        
        if ( currFrame == 5 )
        {
            human->getBodyAnimator()->pushFramesAtoB(&AnimHumanM16A2MoveLoop::getInstance(), 6, 11);
        }
        else if( currFrame == 11 )
        {
            human->getBodyAnimator()->pushFramesAtoB(&AnimHumanM16A2MoveLoop::getInstance(), 0, 5);
        }
    }
    
}

void HumanM16A2MoveLoop::exit(EntityHuman* human)
{
    human->setRunStats(false);
    human->getBodyAnimator()->clearFrameQueue();    }

bool HumanM16A2MoveLoop::onMessage(EntityHuman* human, const Telegram& msg)
{
    if ( msg.msg == MessageType::RELOAD_WEAPON )
    {
        human->getFSM()->changeState(&HumanM16A2Reload::getInstance());
        
        return true;
    }
    return false;
}



//
// HumanM16A2Attack
//
void HumanM16A2Attack::enter(EntityHuman* human)
{
    SoundSource s;
    s.fileName = "M16A2Fire.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 2000.0f;
    human->getGame()->sendMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    human->getGame()->sendMessage(0.12, human, human, MessageType::M16A2_SHOOT, nullptr);
    human->getGame()->sendMessage(0.12, human, human, MessageType::M16A2_SHOOT, nullptr);
    human->getGame()->sendMessage(0.24, human, human, MessageType::M16A2_SHOOT, nullptr);
    
    human->getBodyAnimator()->pushAnimationFrames(&AnimHumanM16A2Attack::getInstance());
    
    human->setStateName("attack");
}

void HumanM16A2Attack::execute(EntityHuman* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if ( human->getBodyAnimator()->isQueueEmpty() )
    {
        human->getFSM()->changeState(&HumanM16A2IdleLoop::getInstance());
    }
    
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
    
}

void HumanM16A2Attack::exit(EntityHuman* human)
{
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanM16A2Attack::onMessage(EntityHuman* human, const Telegram& msg)
{
    if ( msg.msg == MessageType::M16A2_SHOOT )
    {
        if ( human->getEquipedWeapon()->getNumOfLeftRounds() > 0 )
        {
            human->getEquipedWeapon()->attack();
            human->getEquipedWeapon()->setNumOfLeftRounds( human->getEquipedWeapon()->getNumOfLeftRounds() - 1);
            human->getWeaponStatus()->setWeaponStatus(human->getEquipedWeapon());
        }
        
        return true;
    }
    
    return false;
}


//
// HumanM16A2Reload
//
void HumanM16A2Reload::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushAnimationFrames(&AnimHumanM16A2Reload::getInstance());
    
    SoundSource s;
    s.fileName = "M16A2Reload.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 400.0f;
    human->getGame()->sendMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    human->setStateName("reload");
}

void HumanM16A2Reload::execute(EntityHuman* human)
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
        human->getFSM()->changeState(&HumanM16A2IdleLoop::getInstance());
    }
}

void HumanM16A2Reload::exit(EntityHuman* human)
{
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanM16A2Reload::onMessage(EntityHuman* human, const Telegram& msg) { return false; }



//
// HumanM16A2Out
//
void HumanM16A2Out::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushAnimationFrames(&AnimHumanM16A2Out::getInstance());
    
    SoundSource s;
    s.fileName = "GunEnter.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 200.0f;
    human->getGame()->sendMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    human->setStateName("equip weapon");
}

void HumanM16A2Out::execute(EntityHuman* human)
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
        human->getFSM()->changeState(&HumanM16A2IdleLoop::getInstance());
    }
}

void HumanM16A2Out::exit(EntityHuman* human)
{
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanM16A2Out::onMessage(EntityHuman* human, const Telegram& msg) { return false; }



//
// HumanM16A2In
//
void HumanM16A2In::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushAnimationFrames(&AnimHumanM16A2In::getInstance());
    
    SoundSource s;
    s.fileName = "M16A2Enter.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 200.0f;
    human->getGame()->sendMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    human->setStateName("release weapon");
}

void HumanM16A2In::execute(EntityHuman* human)
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

void HumanM16A2In::exit(EntityHuman* human)
{
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanM16A2In::onMessage(EntityHuman* human, const Telegram& msg) { return false; }









