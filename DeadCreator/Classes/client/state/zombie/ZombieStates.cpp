//
//  ZombieStates.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 8. 2..
//
//

#include "ZombieStates.hpp"
#include "HumanBase.hpp"
#include "Animator.hpp"
#include "Animations.hpp"
#include "Game.hpp"
using namespace realtrick::client;
using namespace cocos2d;


//
// ZombieIdleLoop
//
void ZombieIdleLoop::enter(HumanBase* zombie)
{
    zombie->getAnimator()->pushAnimationFrames(&AnimZombieIdleLoop::getInstance());
    zombie->setVelocity( Vec2::ZERO );
    zombie->setStateName("idle");
}


void ZombieIdleLoop::execute(HumanBase* zombie)
{
    int inputMask = zombie->getInputMask();
    Vec2 moving = zombie->getMoving();
    
    if ( zombie->getAnimator()->isQueueEmpty() )
    {
        zombie->getAnimator()->pushAnimationFrames(&AnimZombieIdleLoop::getInstance());
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK) )
    {
        int random = cocos2d::random(0, 1);
        if ( random == 0 ) zombie->getFSM()->changeState(&ZombieAttack1::getInstance());
        else zombie->getFSM()->changeState(&ZombieAttack2::getInstance());
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::TURN) )
    {
        zombie->setVelocity( moving * zombie->getWalkSpeed() );
    }
    
    if ( inputMask == HumanBehaviorType::MOVE )
    {
        zombie->getFSM()->changeState(&ZombieRunLoop::getInstance());
    }
    
    if ( !isMasked(inputMask, HumanBehaviorType::MOVE) )
    {
        zombie->setVelocity( Vec2::ZERO );
    }
}


void ZombieIdleLoop::exit(HumanBase* zombie)
{
    zombie->getAnimator()->clearFrameQueue();
}


bool ZombieIdleLoop::onMessage(HumanBase* zombie, const Telegram& msg)
{
    return false;
}


//
// ZombieRunLoop
//
void ZombieRunLoop::enter(HumanBase* zombie)
{
    zombie->getAnimator()->pushFramesAtoB(&AnimZombieRunLoop::getInstance(), 0, 6);
    zombie->setRunStats(true);
    zombie->setStateName("run");
}


void ZombieRunLoop::execute(HumanBase* zombie)
{
    int inputMask = zombie->getInputMask();
    Vec2 moving = zombie->getMoving();
    int currFrame = zombie->getAnimator()->getFrameIndex();
    
    if( zombie->getAnimator()->isQueueEmpty() )
    {
        zombie->getFSM()->changeState(&ZombieIdleLoop::getInstance());
    }
    
    if ( inputMask == HumanBehaviorType::MOVE )
    {
        zombie->setVelocity( moving * zombie->getRunSpeed() );
        if( moving != Vec2::ZERO ) zombie->setTargetHeading(moving);
        
        if ( currFrame == 6 )
        {
            zombie->getAnimator()->pushFramesAtoB(&AnimZombieRunLoop::getInstance(), 7, 13);
        }
        else if( currFrame == 13 )
        {
            zombie->getAnimator()->pushFramesAtoB(&AnimZombieRunLoop::getInstance(), 0, 6);
        }
    }
}


void ZombieRunLoop::exit(HumanBase* zombie)
{
    zombie->getAnimator()->clearFrameQueue();
    zombie->setRunStats(false);
}


bool ZombieRunLoop::onMessage(HumanBase* zombie, const Telegram& msg)
{
    
    return false;
}


//
// ZombieAttack1
//
void ZombieAttack1::enter(HumanBase* zombie)
{
    zombie->getAnimator()->pushAnimationFrames(&AnimZombieAttack1::getInstance());
    zombie->setStateName("attack");
    
    int rand = cocos2d::random(0, 8);
    if ( rand < 3 )
    {
        SoundSource s;
        s.fileName = std::string("ZombieScream") + _to_string(rand) + ".mp3";
        s.position = zombie->getWorldPosition();
        s.soundRange = 800.0f;
        zombie->getGame()->pushLogic(0.0, MessageType::PLAY_SOUND, &s);
    }
    
    zombie->getGame()->pushLogic(0.0, MessageType::ATTACK_BY_FIST, zombie);
}


void ZombieAttack1::execute(HumanBase* zombie)
{
    int inputMask = zombie->getInputMask();
    Vec2 moving = zombie->getMoving();
    
    if ( zombie->getAnimator()->isQueueEmpty() )
    {
        zombie->getFSM()->changeState(&ZombieIdleLoop::getInstance());
    }
        
    if( isMasked(inputMask, (int)HumanBehaviorType::MOVE) ) zombie->setVelocity( moving * zombie->getWalkSpeed() );
    else zombie->setVelocity( Vec2::ZERO );
}


void ZombieAttack1::exit(HumanBase* zombie)
{
    zombie->getAnimator()->clearFrameQueue();
}


bool ZombieAttack1::onMessage(HumanBase* zombie, const Telegram& msg)
{
    return false;
}


//
// ZombieAttack2
//
void ZombieAttack2::enter(HumanBase* zombie)
{
    zombie->getAnimator()->pushAnimationFrames(&AnimZombieAttack2::getInstance());
    zombie->setStateName("attack");
    
    int rand = cocos2d::random(0, 8);
    if ( rand < 3 )
    {
        SoundSource s;
        s.fileName = std::string("ZombieScream") + _to_string(rand) + ".mp3";
        s.position = zombie->getWorldPosition();
        s.soundRange = 800.0f;
        zombie->getGame()->pushLogic(0.0, MessageType::PLAY_SOUND, &s);
    }
    zombie->getGame()->pushLogic(0.0, MessageType::ATTACK_BY_FIST, zombie);
}


void ZombieAttack2::execute(HumanBase* zombie)
{
    int inputMask = zombie->getInputMask();
    Vec2 moving = zombie->getMoving();
    
    if ( zombie->getAnimator()->isQueueEmpty() )
    {
        zombie->getFSM()->changeState(&ZombieIdleLoop::getInstance());
    }
    
    if( isMasked(inputMask, (int)HumanBehaviorType::MOVE) ) zombie->setVelocity( moving * zombie->getWalkSpeed() );
    else zombie->setVelocity( Vec2::ZERO );
}


void ZombieAttack2::exit(HumanBase* zombie)
{
    zombie->getAnimator()->clearFrameQueue();
}


bool ZombieAttack2::onMessage(HumanBase* zombie, const Telegram& msg)
{
    return false;
}


//
// ZombieDead
//
void ZombieDead::enter(HumanBase* zombie)
{
    zombie->setLocalZOrder(Z_ORDER_HUMAN - 1);
    zombie->getGame()->sendMessage(cocos2d::random(3.0, 7.0), zombie, zombie, MessageType::DIE, nullptr);
    zombie->setDead();
    
    zombie->getAnimator()->setShadowVisible(false);
    
    zombie->getAnimator()->pushAnimationFrames(&AnimZombieDead::getInstance());
    zombie->setStateName("dead");
    
    SoundSource s;
    s.fileName = std::string("ZombieDie") + _to_string(cocos2d::random(0, 2)) + ".mp3";
    s.position = zombie->getWorldPosition();
    s.soundRange = 800.0f;
    zombie->getGame()->pushLogic(0.0, MessageType::PLAY_SOUND, &s);
}


void ZombieDead::execute(HumanBase* zombie)
{
    
}


void ZombieDead::exit(HumanBase* zombie)
{
    zombie->getAnimator()->clearFrameQueue();
}


bool ZombieDead::onMessage(HumanBase* zombie, const Telegram& msg)
{
    if ( msg.msg == MessageType::DIE )
    {
//        zombie->getGame()->removeEntity(zombie);
        
        return true;
    }
    
    return false;
}









