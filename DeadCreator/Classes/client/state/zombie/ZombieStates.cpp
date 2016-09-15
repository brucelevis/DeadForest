//
//  ZombieStates.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 2..
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
// ZombieAttack
//
void ZombieAttack::enter(HumanBase* zombie)
{
    zombie->getAnimator()->pushAnimationFrames(&AnimZombieAttack::getInstance());
    zombie->setStateName("attack");
}


void ZombieAttack::execute(HumanBase* zombie)
{
}


void ZombieAttack::exit(HumanBase* zombie)
{
    zombie->getAnimator()->clearFrameQueue();
}


bool ZombieAttack::onMessage(HumanBase* zombie, const Telegram& msg)
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









