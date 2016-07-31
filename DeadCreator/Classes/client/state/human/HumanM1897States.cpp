//
//  HumanM1897States.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 14..
//
//

#include "HumanM1897States.hpp"
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
// HumanM1897IdleLoop
//
void HumanM1897IdleLoop::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushAnimationFrames(&AnimHumanM1897IdleLoop::getInstance());
    human->setVelocity( Vec2::ZERO );
    human->setStateName("idle");
}

void HumanM1897IdleLoop::execute(EntityHuman* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if(human->getBodyAnimator()->isQueueEmpty())
    {
        human->getBodyAnimator()->pushAnimationFrames(&AnimHumanM1897IdleLoop::getInstance());
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
                human->getFSM()->changeState(&HumanM1897Attack::getInstance());
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
        human->getFSM()->changeState(&HumanM1897MoveLoop::getInstance());
    }
    
    if ( !isMasked(inputMask, HumanBehaviorType::RUN) )
    {
        human->setVelocity( Vec2::ZERO );
    }
}

void HumanM1897IdleLoop::exit(EntityHuman* human)
{
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanM1897IdleLoop::onMessage(EntityHuman* human, const Telegram& msg)
{
    if ( msg.msg == MessageType::RELOAD_WEAPON )
    {
        human->getFSM()->changeState(&HumanM1897Reload::getInstance());
        return true;
    }
    
    return false;
}



//
// HumanM1897MoveLoop
//
void HumanM1897MoveLoop::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushFramesAtoB(&AnimHumanM1897MoveLoop::getInstance(), 0, 7);
    human->setRunStats(true);
    
    human->setStateName("run");
}

void HumanM1897MoveLoop::execute(EntityHuman* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    int currFrame = human->getBodyAnimator()->getFrameIndex();
    
    if( human->getBodyAnimator()->isQueueEmpty() )
    {
        human->getFSM()->changeState(&HumanM1897IdleLoop::getInstance());
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK_ENDED) )
    {
        human->removeInputMask(HumanBehaviorType::ATTACK_ENDED);
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK_BEGAN) || isMasked(inputMask, HumanBehaviorType::TURN) )
    {
        if ( currFrame == 7 || currFrame == 15 )
        {
            human->getFSM()->changeState(&HumanM1897IdleLoop::getInstance());
        }
    }
    
    if ( inputMask == HumanBehaviorType::RUN )
    {
        human->setVelocity( moving * human->getRunSpeed() );
        if( moving != Vec2::ZERO ) human->setTargetHeading(moving);
        
        if ( currFrame == 7 )
        {
            human->getBodyAnimator()->pushFramesAtoB(&AnimHumanM1897MoveLoop::getInstance(), 8, 15);
        }
        else if( currFrame == 15 )
        {
            human->getBodyAnimator()->pushFramesAtoB(&AnimHumanM1897MoveLoop::getInstance(), 0, 7);
        }
    }
    
}

void HumanM1897MoveLoop::exit(EntityHuman* human)
{
    human->setRunStats(false);
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanM1897MoveLoop::onMessage(EntityHuman* human, const Telegram& msg)
{
    if ( msg.msg == MessageType::RELOAD_WEAPON )
    {
        human->getFSM()->changeState(&HumanM1897Reload::getInstance());
        
        return true;
    }
    return false;
}



//
// HumanM1897Attack
//
void HumanM1897Attack::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushFramesAtoB(&AnimHumanM1897Attack::getInstance(), 0, 3);
    human->setStateName("attack");
}

void HumanM1897Attack::execute(EntityHuman* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    int currFrame = human->getBodyAnimator()->getFrameIndex();
    
    if ( human->getBodyAnimator()->isQueueEmpty() )
    {
        human->getFSM()->changeState(&HumanM1897IdleLoop::getInstance());
    }
    
    if( isMasked(inputMask, (int)HumanBehaviorType::RUN) )
    {
        human->setVelocity( moving * human->getWalkSpeed() );
    }
    else
    {
        human->setVelocity( Vec2::ZERO );
    }
    if ( currFrame < 3 )
    {
        if ( isMasked(inputMask, (int)HumanBehaviorType::ATTACK_ENDED) )
        {
            human->removeInputMask(HumanBehaviorType::ATTACK_ENDED);
            human->getBodyAnimator()->pushFramesAtoB(&AnimHumanM1897Attack::getInstance(), currFrame, 0);
        }
    }
    else if ( currFrame == 3 )
    {
        if ( isMasked(inputMask, (int)HumanBehaviorType::ATTACK_BEGAN) )
        {
            human->getBodyAnimator()->enableForceStop(true);
        }
        else if ( isMasked(inputMask, (int)HumanBehaviorType::ATTACK_ENDED) )
        {
            human->removeInputMask(HumanBehaviorType::ATTACK_ENDED);
            human->getBodyAnimator()->enableForceStop(false);
            human->getBodyAnimator()->pushFramesAtoB(&AnimHumanM1897Attack::getInstance(), 4, 16);
            
            SoundSource s;
            s.fileName = "shotgun.mp3";
            s.position = human->getWorldPosition();
            s.soundRange = 2000.0f;
            human->getGame()->sendMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
            
            human->getEquipedWeapon()->attack();
            human->getEquipedWeapon()->setNumOfLeftRounds( human->getEquipedWeapon()->getNumOfLeftRounds() - 1);
            human->getWeaponStatus()->setWeaponStatus(human->getEquipedWeapon());
        }
    }
    
}

void HumanM1897Attack::exit(EntityHuman* human)
{
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanM1897Attack::onMessage(EntityHuman* human, const Telegram& msg) { return false; }


//
// HumanM1897Reload
//
void HumanM1897Reload::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushAnimationFrames(&AnimHumanM1897Reload::getInstance());
    
    SoundSource s;
    s.fileName = "shotgun_reload.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 400.0f;
    human->getGame()->sendMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    human->setStateName("reload");
}

void HumanM1897Reload::execute(EntityHuman* human)
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
        human->getFSM()->changeState(&HumanM1897IdleLoop::getInstance());
    }
}

void HumanM1897Reload::exit(EntityHuman* human)
{
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanM1897Reload::onMessage(EntityHuman* human, const Telegram& msg) { return false; }



//
// HumanM1897Out
//
void HumanM1897Out::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushAnimationFrames(&AnimHumanM1897Out::getInstance());
    
    SoundSource s;
    s.fileName = "GunEnter.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 200.0f;
    human->getGame()->sendMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    human->setStateName("equip weapon");
}

void HumanM1897Out::execute(EntityHuman* human)
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
        human->getFSM()->changeState(&HumanM1897IdleLoop::getInstance());
    }
}

void HumanM1897Out::exit(EntityHuman* human)
{
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanM1897Out::onMessage(EntityHuman* human, const Telegram& msg) { return false; }



//
// HumanM1897In
//
void HumanM1897In::enter(EntityHuman* human)
{
    human->getBodyAnimator()->pushAnimationFrames(&AnimHumanM1897In::getInstance());
    
    SoundSource s;
    s.fileName = "M16A2Enter.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 200.0f;
    human->getGame()->sendMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    human->setStateName("release weapon");
}

void HumanM1897In::execute(EntityHuman* human)
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

void HumanM1897In::exit(EntityHuman* human)
{
    human->getBodyAnimator()->clearFrameQueue();
}

bool HumanM1897In::onMessage(EntityHuman* human, const Telegram& msg) { return false; }









