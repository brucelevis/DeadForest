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
#include "HumanBase.hpp"
#include "HumanOwnedAnimations.hpp"
#include "Game.hpp"
#include "WeaponBase.hpp"
#include "UiLayer.hpp"
#include "Items.hpp"
using namespace cocos2d;
using namespace realtrick::client;


//
// HumanM1897IdleLoop
//
void HumanM1897IdleLoop::enter(HumanBase* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanM1897IdleLoop::getInstance());
    human->setVelocity( Vec2::ZERO );
    human->setStateName("idle");
}

void HumanM1897IdleLoop::execute(HumanBase* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if(human->getAnimator()->isQueueEmpty())
    {
        human->getAnimator()->pushAnimationFrames(&AnimHumanM1897IdleLoop::getInstance());
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
                human->getFSM()->changeState(&HumanM1897AttackAction::getInstance());
            }
            // 2. 남아있는 총알이 없음.
            else
            {
                SoundSource s;
                s.fileName = "Glock17Empty.mp3";
                s.position = human->getWorldPosition();
                s.soundRange = 200.0f;
                human->getGame()->pushLogic(0.0, MessageType::PLAY_SOUND, &s);
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
    
    if ( inputMask == HumanBehaviorType::MOVE )
    {
        human->getFSM()->changeState(&HumanM1897MoveLoop::getInstance());
    }
    
    if ( !isMasked(inputMask, HumanBehaviorType::MOVE) )
    {
        human->setVelocity( Vec2::ZERO );
    }
}

void HumanM1897IdleLoop::exit(HumanBase* human)
{
    human->getAnimator()->clearFrameQueue();
}

bool HumanM1897IdleLoop::onMessage(HumanBase* human, const Telegram& msg)
{
    if ( msg.msg == MessageType::RELOAD_WEAPON )
    {
        int bulletNum = *static_cast<int*>(msg.extraInfo);
        human->getEquipedWeapon()->setReservedBullets(bulletNum);
        
        double animatedTime = AnimHumanM1897Reload::getInstance().getMaxFrame() * AnimHumanM1897Reload::getInstance().getFrameSwapTime();
        human->getGame()->sendMessage(animatedTime, human, human, MessageType::RELOAD_COMPLETE, nullptr);
        
        human->getFSM()->changeState(&HumanM1897Reload::getInstance());
        return true;
    }
    
    return false;
}



//
// HumanM1897MoveLoop
//
void HumanM1897MoveLoop::enter(HumanBase* human)
{
    human->getAnimator()->pushFramesAtoB(&AnimHumanM1897MoveLoop::getInstance(), 0, 7);
    human->setRunStats(true);
    
    human->setStateName("run");
}

void HumanM1897MoveLoop::execute(HumanBase* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    int currFrame = human->getAnimator()->getFrameIndex();
    
    if( human->getAnimator()->isQueueEmpty() )
    {
        human->getFSM()->changeState(&HumanM1897IdleLoop::getInstance());
    }
    
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK) || isMasked(inputMask, HumanBehaviorType::TURN) )
    {
        if ( currFrame == 7 || currFrame == 15 )
        {
            human->getFSM()->changeState(&HumanM1897IdleLoop::getInstance());
        }
    }
    
    if ( inputMask == HumanBehaviorType::MOVE )
    {
        human->setVelocity( moving * human->getRunSpeed() );
        if( moving != Vec2::ZERO ) human->setTargetHeading(moving);
        
        if ( currFrame == 7 )
        {
            human->getAnimator()->pushFramesAtoB(&AnimHumanM1897MoveLoop::getInstance(), 8, 15);
        }
        else if( currFrame == 15 )
        {
            human->getAnimator()->pushFramesAtoB(&AnimHumanM1897MoveLoop::getInstance(), 0, 7);
        }
    }
    
}

void HumanM1897MoveLoop::exit(HumanBase* human)
{
    human->setRunStats(false);
    human->getAnimator()->clearFrameQueue();
}

bool HumanM1897MoveLoop::onMessage(HumanBase* human, const Telegram& msg)
{
    if ( msg.msg == MessageType::RELOAD_WEAPON )
    {
        int bulletNum = *static_cast<int*>(msg.extraInfo);
        human->getEquipedWeapon()->setReservedBullets(bulletNum);
        
        double animatedTime = AnimHumanM1897Reload::getInstance().getMaxFrame() * AnimHumanM1897Reload::getInstance().getFrameSwapTime();
        human->getGame()->sendMessage(animatedTime, human, human, MessageType::RELOAD_COMPLETE, nullptr);
        
        human->getFSM()->changeState(&HumanM1897Reload::getInstance());
        
        return true;
    }
    
    return false;
}


//
// HumanM1897Reload
//
void HumanM1897Reload::enter(HumanBase* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanM1897Reload::getInstance());
    
    SoundSource s;
    s.fileName = "shotgun_reload.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 400.0f;
    human->getGame()->pushLogic(0.0, MessageType::PLAY_SOUND, &s);
    
    human->setStateName("reload");
}

void HumanM1897Reload::execute(HumanBase* human)
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
        human->getFSM()->changeState(&HumanM1897IdleLoop::getInstance());
    }
}

void HumanM1897Reload::exit(HumanBase* human)
{
    human->getAnimator()->clearFrameQueue();
}

bool HumanM1897Reload::onMessage(HumanBase* human, const Telegram& msg) { return false; }



//
// HumanM1897Out
//
void HumanM1897Out::enter(HumanBase* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanM1897Out::getInstance());
    
    SoundSource s;
    s.fileName = "GunEnter.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 200.0f;
    human->getGame()->pushLogic(0.0, MessageType::PLAY_SOUND, &s);
    
    human->setStateName("equip weapon");
}

void HumanM1897Out::execute(HumanBase* human)
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
        human->getFSM()->changeState(&HumanM1897IdleLoop::getInstance());
    }
}

void HumanM1897Out::exit(HumanBase* human)
{
    human->getAnimator()->clearFrameQueue();
}

bool HumanM1897Out::onMessage(HumanBase* human, const Telegram& msg) { return false; }



//
// HumanM1897In
//
void HumanM1897In::enter(HumanBase* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanM1897In::getInstance());
    
    SoundSource s;
    s.fileName = "M16A2Enter.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 200.0f;
    human->getGame()->pushLogic(0.0, MessageType::PLAY_SOUND, &s);
    
    human->setStateName("release weapon");
}

void HumanM1897In::execute(HumanBase* human)
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

void HumanM1897In::exit(HumanBase* human)
{
    human->getAnimator()->clearFrameQueue();
}

bool HumanM1897In::onMessage(HumanBase* human, const Telegram& msg) { return false; }


//
// HumanM1897AttackReady
//
void HumanM1897AttackReady::enter(HumanBase* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanM1897AttackReady::getInstance());
    human->setStateName("lifting M1897");
}

void HumanM1897AttackReady::execute(HumanBase* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if ( human->getAnimator()->isQueueEmpty() )
    {
        human->getFSM()->changeState(&HumanM1897AttackAction::getInstance());
    }
    
    if( isMasked(inputMask, (int)HumanBehaviorType::MOVE) ) human->setVelocity( moving * human->getWalkSpeed() );
    else human->setVelocity( Vec2::ZERO );
}

void HumanM1897AttackReady::exit(HumanBase* human)
{
    human->getAnimator()->clearFrameQueue();
}


//
// HumanM1897AttackRelease
//
void HumanM1897AttackRelease::enter(HumanBase* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanM1897AttackRelease::getInstance());
    human->setStateName("take down m1897");
}

void HumanM1897AttackRelease::execute(HumanBase* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if ( human->getAnimator()->isQueueEmpty() )
    {
        human->getFSM()->changeState(&HumanM1897IdleLoop::getInstance());
    }
    
    if( isMasked(inputMask, (int)HumanBehaviorType::MOVE) ) human->setVelocity( moving * human->getWalkSpeed() );
    else human->setVelocity( Vec2::ZERO );
}

void HumanM1897AttackRelease::exit(HumanBase* human)
{
    human->getAnimator()->clearFrameQueue();
}


//
// HumanM1897AttackHover
//
void HumanM1897AttackHover::enter(HumanBase* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanM1897AttackHover::getInstance());
    human->setStateName("aimming...");
}

void HumanM1897AttackHover::execute(HumanBase* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if ( human->getAnimator()->isQueueEmpty() )
    {
        if ( isMasked(inputMask, HumanBehaviorType::ATTACK ) )
        {
            human->getFSM()->changeState(&HumanM1897AttackHover::getInstance());
        }
    }
    
    if ( !isMasked(inputMask, HumanBehaviorType::ATTACK ) )
    {
        human->getFSM()->changeState(&HumanM1897AttackAction::getInstance());
    }
    
    if( isMasked(inputMask, (int)HumanBehaviorType::MOVE) ) human->setVelocity( moving * human->getWalkSpeed() );
    else human->setVelocity( Vec2::ZERO );
}

void HumanM1897AttackHover::exit(HumanBase* human)
{
    human->getAnimator()->clearFrameQueue();
}


//
// HumanM1897AttackAction
//
void HumanM1897AttackAction::enter(HumanBase* human)
{
    human->getAnimator()->pushAnimationFrames(&AnimHumanM1897AttackAction::getInstance());
    human->setStateName("attack");
    
    SoundSource s;
    s.fileName = "shotgun.mp3";
    s.position = human->getWorldPosition();
    s.soundRange = 2000.0f;
    human->getGame()->pushLogic(0.0, MessageType::PLAY_SOUND, &s);
    
    human->getGame()->pushLogic(0.0, MessageType::ATTACK_BY_WEAPON, human);
}

void HumanM1897AttackAction::execute(HumanBase* human)
{
    int inputMask = human->getInputMask();
    Vec2 moving = human->getMoving();
    
    if ( human->getAnimator()->isQueueEmpty() )
    {
        human->getFSM()->changeState(&HumanM1897IdleLoop::getInstance());
    }
    
    if( isMasked(inputMask, (int)HumanBehaviorType::MOVE) ) human->setVelocity( moving * human->getWalkSpeed() );
    else human->setVelocity( Vec2::ZERO );
    
}

void HumanM1897AttackAction::exit(HumanBase* human)
{
    human->getAnimator()->clearFrameQueue();
}









