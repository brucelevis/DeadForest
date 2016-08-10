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
#include "HumanBase.hpp"
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
void HumanGlock17IdleLoop::enter(HumanBase* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanGlock17IdleLoop::getInstance());
    human->setVelocity( Vec2::ZERO );
    human->setStateName("idle");
}

void HumanGlock17IdleLoop::execute(HumanBase* human)
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
        if ( static_cast<EntityPlayer*>(human)->getEquipedWeapon()->isReadyToAttack() )
        {
            // 1. 남아있는 총알이 있음.
            if ( static_cast<EntityPlayer*>(human)->getEquipedWeapon()->getNumOfLeftRounds() > 0 )
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
            
            WeaponBase* weapon = static_cast<EntityPlayer*>(human)->getEquipedWeapon();
            human->getGame()->sendMessage(weapon->getDelay(), human, human, MessageType::WEAPON_READY, reinterpret_cast<void*>(weapon));
            
            static_cast<EntityPlayer*>(human)->getEquipedWeapon()->enableReadyToAttack(false);
        }
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::TURN) )
    {
        human->setVelocity( moving * human->getWalkSpeed() );
    }
    
    if ( inputMask == HumanBehaviorType::MOVE )
    {
        human->getFSM()->changeState(&HumanGlock17MoveLoop::getInstance());
    }
    
    if ( !isMasked(inputMask, HumanBehaviorType::MOVE) )
    {
        human->setVelocity( Vec2::ZERO );
    }
}

void HumanGlock17IdleLoop::exit(HumanBase* human)
{
    human->getAnimator()->clearFrameQueue();
}

bool HumanGlock17IdleLoop::onMessage(HumanBase* human, const Telegram& msg)
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
void HumanGlock17MoveLoop::enter(HumanBase* human)
{
    human->getAnimator()->pushFramesAtoB(&AnimHumanGlock17MoveLoop::getInstance(), 0, 5);
    human->setRunStats(true);
    human->setStateName("run");
}

void HumanGlock17MoveLoop::execute(HumanBase* human)
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
    
    if ( inputMask == HumanBehaviorType::MOVE )
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

void HumanGlock17MoveLoop::exit(HumanBase* human)
{
    human->setRunStats(false);
    human->getAnimator()->clearFrameQueue();    }

bool HumanGlock17MoveLoop::onMessage(HumanBase* human, const Telegram& msg)
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
void HumanGlock17Attack::enter(HumanBase* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanGlock17Attack::getInstance());
    
    SoundSource s;
    s.fileName = "Glock17Fire" + _to_string(random(0, 2)) + ".mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 2000.0f;
    human->getGame()->sendMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    static_cast<EntityPlayer*>(human)->getEquipedWeapon()->attack();
    static_cast<EntityPlayer*>(human)->getEquipedWeapon()->setNumOfLeftRounds( static_cast<EntityPlayer*>(human)->getEquipedWeapon()->getNumOfLeftRounds() - 1);
    static_cast<EntityPlayer*>(human)->getWeaponStatus()->setWeaponStatus(static_cast<EntityPlayer*>(human)->getEquipedWeapon());
    
    human->setStateName("attack");
}

void HumanGlock17Attack::execute(HumanBase* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if( isMasked(inputMask, HumanBehaviorType::MOVE) )
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

void HumanGlock17Attack::exit(HumanBase* human)
{
    human->getAnimator()->clearFrameQueue();
}

bool HumanGlock17Attack::onMessage(HumanBase* human, const Telegram& msg) { return false; }


//
// HumanGlock17Reload
//
void HumanGlock17Reload::enter(HumanBase* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanGlock17Reload::getInstance());
    
    SoundSource s;
    s.fileName = "Glock17Reload.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 400.0f;
    human->getGame()->sendMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    human->setStateName("reload");
}

void HumanGlock17Reload::execute(HumanBase* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if( isMasked(inputMask, HumanBehaviorType::MOVE) )
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

void HumanGlock17Reload::exit(HumanBase* human)
{
    human->getAnimator()->clearFrameQueue();
}

bool HumanGlock17Reload::onMessage(HumanBase* human, const Telegram& msg) { return false; }



//
// HumanGlock17Out
//
void HumanGlock17Out::enter(HumanBase* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanGlock17Out::getInstance());
    
    SoundSource s;
    s.fileName = "GunEnter.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 200.0f;
    human->getGame()->sendMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    human->setStateName("equip weapon");
}

void HumanGlock17Out::execute(HumanBase* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if( isMasked(inputMask, HumanBehaviorType::MOVE) )
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

void HumanGlock17Out::exit(HumanBase* human)
{
    human->getAnimator()->clearFrameQueue();
}

bool HumanGlock17Out::onMessage(HumanBase* human, const Telegram& msg) { return false; }



//
// HumanGlock17In
//
void HumanGlock17In::enter(HumanBase* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanGlock17In::getInstance());
    
    SoundSource s;
    s.fileName = "M16A2Enter.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 200.0f;
    human->getGame()->sendMessage(0.0, human, human, MessageType::PLAY_SOUND, &s);
    
    human->setStateName("release weapon");
}

void HumanGlock17In::execute(HumanBase* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if( isMasked(inputMask, HumanBehaviorType::MOVE) )
    {
        human->setVelocity( moving * human->getWalkSpeed() );
    }
    else
    {
        human->setVelocity( Vec2::ZERO );
    }
    
    if(human->getAnimator()->isQueueEmpty())
    {
        if ( static_cast<EntityPlayer*>(human)->getEquipedWeapon() == nullptr )
        {
            // 무기가 없으면 주먹 상태로
            human->getFSM()->changeState(&HumanFistOut::getInstance());
        }
        else
        {
            // 있으면 해당무기를 꺼냄.
            static_cast<EntityPlayer*>(human)->getEquipedWeapon()->outWeapon();
        }
    }
}

void HumanGlock17In::exit(HumanBase* human)
{
    human->getAnimator()->clearFrameQueue();
}

bool HumanGlock17In::onMessage(HumanBase* human, const Telegram& msg) { return false; }









