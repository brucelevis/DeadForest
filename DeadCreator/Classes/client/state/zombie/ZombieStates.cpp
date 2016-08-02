//
//  ZombieStates.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 2..
//
//

#include "ZombieStates.hpp"
#include "EntityZombie.hpp"
#include "Animator.hpp"
#include "ZombieAnimations.hpp"
using namespace realtrick::client;
using namespace cocos2d;


//
// ZombieIdleLoop
//
void ZombieIdleLoop::enter(EntityZombie* zombie)
{
    zombie->getAnimator()->pushAnimationFrames(&AnimZombieIdleLoop::getInstance());
    zombie->setVelocity( Vec2::ZERO );
    zombie->setStateName("idle");
}


void ZombieIdleLoop::execute(EntityZombie* zombie)
{
    if ( zombie->getAnimator()->isQueueEmpty() )
    {
        zombie->getAnimator()->pushAnimationFrames(&AnimZombieIdleLoop::getInstance());
    }
}


void ZombieIdleLoop::exit(EntityZombie* zombie)
{
    zombie->getAnimator()->clearFrameQueue();
}


bool ZombieIdleLoop::onMessage(EntityZombie* zombie, const Telegram& msg)
{
    return false;
}


//
// ZombieRunLoop
//
void ZombieRunLoop::enter(EntityZombie* zombie)
{
    zombie->getAnimator()->pushAnimationFrames(&AnimZombieIdleLoop::getInstance());
    zombie->setRunStats(true);
    zombie->setStateName("run");
}


void ZombieRunLoop::execute(EntityZombie* zombie)
{
}


void ZombieRunLoop::exit(EntityZombie* zombie)
{
    zombie->getAnimator()->clearFrameQueue();
    zombie->setRunStats(false);
}


bool ZombieRunLoop::onMessage(EntityZombie* zombie, const Telegram& msg)
{
    
    return false;
}


//
// ZombieAttack
//
void ZombieAttack::enter(EntityZombie* zombie)
{
    zombie->getAnimator()->pushAnimationFrames(&AnimZombieAttack::getInstance());
    zombie->setStateName("attack");
}


void ZombieAttack::execute(EntityZombie* zombie)
{
}


void ZombieAttack::exit(EntityZombie* zombie)
{
    zombie->getAnimator()->clearFrameQueue();
}


bool ZombieAttack::onMessage(EntityZombie* zombie, const Telegram& msg)
{
    return false;
}


//
// ZombieDead
//
void ZombieDead::enter(EntityZombie* zombie)
{
    zombie->getAnimator()->pushAnimationFrames(&AnimZombieDead::getInstance());
    zombie->setStateName("dead");
}


void ZombieDead::execute(EntityZombie* zombie)
{
    
}


void ZombieDead::exit(EntityZombie* zombie)
{
    zombie->getAnimator()->clearFrameQueue();
}


bool ZombieDead::onMessage(EntityZombie* zombie, const Telegram& msg)
{
    return false;
}









