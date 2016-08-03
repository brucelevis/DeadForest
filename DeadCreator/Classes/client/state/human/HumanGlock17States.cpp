//
//  HumanGlock17States.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 14..
//
//

#include "HumanGlock17States.hpp"
#include "Telegram.hpp"
#include "MessageTypes.hpp"
#include "MessageDispatcher.hpp"
#include "EntityPlayer.hpp"
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
// HumanGlock17IdleLoop
//
void HumanGlock17IdleLoop::enter(EntityPlayer* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanGlock17IdleLoop::getInstance());
    human->setVelocity( Vec2::ZERO );
    human->setStateName("idle");
}

void HumanGlock17IdleLoop::execute(EntityPlayer* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if(human->getAnimator()->isQueueEmpty())
    {
        human->getAnimator()->pushAnimationFrames(&AnimHumanGlock17IdleLoop::getInstance());
    }
    
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK) )
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
        human->getFSM()->changeState(&HumanGlock17MoveLoop::getInstance());
    }
    
    if ( !isMasked(inputMask, HumanBehaviorType::RUN) )
    {
        human->setVelocity( Vec2::ZERO );
    }
}

void HumanGlock17IdleLoop::exit(EntityPlayer* human)
{
    human->getAnimator()->clearFrameQueue();
}

bool HumanGlock17IdleLoop::onMessage(EntityPlayer* human, const Telegram& msg)
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
void HumanGlock17MoveLoop::enter(EntityPlayer* human)
{
    human->getAnimator()->pushFramesAtoB(&AnimHumanGlock17MoveLoop::getInstance(), 0, 5);
    human->setRunStats(true);
    human->setStateName("run");
}

void HumanGlock17MoveLoop::execute(EntityPlayer* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    int currFrame = human->getAnimator()->getFrameIndex();
    
    if( human->getAnimator()->isQueueEmpty() )
    {
        human->getFSM()->changeState(&HumanGlock17IdleLoop::getInstance());
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK) || isMasked(inputMask, HumanBehaviorType::TURN) )
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
            human->getAnimator()->pushFramesAtoB(&AnimHumanGlock17MoveLoop::getInstance(), 6, 11);
        }
        else if( currFrame == 11 )
        {
            human->getAnimator()->pushFramesAtoB(&AnimHumanGlock17MoveLoop::getInstance(), 0, 5);
        }
    }
}

void HumanGlock17MoveLoop::exit(EntityPlayer* human)
{
    human->setRunStats(false);
    human->getAnimator()->clearFrameQueue();    }

bool HumanGlock17MoveLoop::onMessage(EntityPlayer* human, const Telegram& msg)
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
void HumanGlock17Attack::enter(EntityPlayer* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanGlock17Attack::getInstance());
    
    SoundSource s;
    s.fileName = "Glock17Fire" + _to_string(random(0, 2)) + ".mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 2000.0f;
    human->getGame()->sendMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    human->getEquipedWeapon()->attack();
    human->getEquipedWeapon()->setNumOfLeftRounds( human->getEquipedWeapon()->getNumOfLeftRounds() - 1);
    human->getWeaponStatus()->setWeaponStatus(human->getEquipedWeapon());
    
    human->setStateName("attack");
}

void HumanGlock17Attack::execute(EntityPlayer* human)
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
    
    if ( human->getAnimator()->isQueueEmpty() )
    {
        human->getFSM()->changeState(&HumanGlock17IdleLoop::getInstance());
    }
}

void HumanGlock17Attack::exit(EntityPlayer* human)
{
    human->getAnimator()->clearFrameQueue();
}

bool HumanGlock17Attack::onMessage(EntityPlayer* human, const Telegram& msg) { return false; }


//
// HumanGlock17Reload
//
void HumanGlock17Reload::enter(EntityPlayer* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanGlock17Reload::getInstance());
    
    SoundSource s;
    s.fileName = "Glock17Reload.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 400.0f;
    human->getGame()->sendMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    human->setStateName("reload");
}

void HumanGlock17Reload::execute(EntityPlayer* human)
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
    
    if ( human->getAnimator()->isQueueEmpty() )
    {
        human->getFSM()->changeState(&HumanGlock17IdleLoop::getInstance());
    }
}

void HumanGlock17Reload::exit(EntityPlayer* human)
{
    human->getAnimator()->clearFrameQueue();
}

bool HumanGlock17Reload::onMessage(EntityPlayer* human, const Telegram& msg) { return false; }



//
// HumanGlock17Out
//
void HumanGlock17Out::enter(EntityPlayer* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanGlock17Out::getInstance());
    
    SoundSource s;
    s.fileName = "GunEnter.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 200.0f;
    human->getGame()->sendMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    human->setStateName("equip weapon");
}

void HumanGlock17Out::execute(EntityPlayer* human)
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
    
    if(human->getAnimator()->isQueueEmpty())
    {
        human->getFSM()->changeState(&HumanGlock17IdleLoop::getInstance());
    }
}

void HumanGlock17Out::exit(EntityPlayer* human)
{
    human->getAnimator()->clearFrameQueue();
}

bool HumanGlock17Out::onMessage(EntityPlayer* human, const Telegram& msg) { return false; }



//
// HumanGlock17In
//
void HumanGlock17In::enter(EntityPlayer* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanGlock17In::getInstance());
    
    SoundSource s;
    s.fileName = "M16A2Enter.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 200.0f;
    human->getGame()->sendMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    human->setStateName("release weapon");
}

void HumanGlock17In::execute(EntityPlayer* human)
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

void HumanGlock17In::exit(EntityPlayer* human)
{
    human->getAnimator()->clearFrameQueue();
}

bool HumanGlock17In::onMessage(EntityPlayer* human, const Telegram& msg) { return false; }









