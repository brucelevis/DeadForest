//
//  Zombie3States.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 10. 4..
//
//

#include "Zombie3States.hpp"
#include "HumanBase.hpp"
#include "Animator.hpp"
#include "Animations.hpp"
#include "Game.hpp"
using namespace realtrick::client;
using namespace cocos2d;


//
// Zombie3IdleLoop
//
void Zombie3IdleLoop::enter(HumanBase* zombie)
{
    zombie->getAnimator()->pushAnimationFrames(&AnimZombie3IdleLoop::getInstance());
    zombie->setVelocity( Vec2::ZERO );
    zombie->setStateName("idle");
}


void Zombie3IdleLoop::execute(HumanBase* zombie)
{
    int inputMask = zombie->getInputMask();
    Vec2 moving = zombie->getMoving();
    
    if ( zombie->getAnimator()->isQueueEmpty() )
    {
        zombie->getAnimator()->pushAnimationFrames(&AnimZombie3IdleLoop::getInstance());
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK) )
    {
        int random = cocos2d::random(0, 1);
        if ( random == 0 ) zombie->getFSM()->changeState(&Zombie3Attack1::getInstance());
        else zombie->getFSM()->changeState(&Zombie3Attack2::getInstance());
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::TURN) )
    {
        zombie->setVelocity( moving * zombie->getWalkSpeed() );
    }
    
    if ( inputMask == HumanBehaviorType::MOVE )
    {
        zombie->getFSM()->changeState(&Zombie3RunLoop::getInstance());
    }
    
    if ( !isMasked(inputMask, HumanBehaviorType::MOVE) )
    {
        zombie->setVelocity( Vec2::ZERO );
    }
}


void Zombie3IdleLoop::exit(HumanBase* zombie)
{
    zombie->getAnimator()->clearFrameQueue();
}


bool Zombie3IdleLoop::onMessage(HumanBase* zombie, const Telegram& msg)
{
    return false;
}


//
// Zombie3RunLoop
//
void Zombie3RunLoop::enter(HumanBase* zombie)
{
    zombie->getAnimator()->pushFramesAtoB(&AnimZombie3RunLoop::getInstance(), 0, 6);
    zombie->setRunStats(true);
    zombie->setStateName("run");
}


void Zombie3RunLoop::execute(HumanBase* zombie)
{
    int inputMask = zombie->getInputMask();
    Vec2 moving = zombie->getMoving();
    int currFrame = zombie->getAnimator()->getFrameIndex();
    
    if( zombie->getAnimator()->isQueueEmpty() )
    {
        zombie->getFSM()->changeState(&Zombie3IdleLoop::getInstance());
    }
    
    if ( inputMask == HumanBehaviorType::MOVE )
    {
        zombie->setVelocity( moving * zombie->getRunSpeed() );
        if( moving != Vec2::ZERO ) zombie->setTargetHeading(moving);
        
        if ( currFrame == 4 )
        {
            zombie->getAnimator()->pushFramesAtoB(&AnimZombie3RunLoop::getInstance(), 5, 9);
        }
        else if( currFrame == 9 )
        {
            zombie->getAnimator()->pushFramesAtoB(&AnimZombie3RunLoop::getInstance(), 0, 4);
        }
    }
}


void Zombie3RunLoop::exit(HumanBase* zombie)
{
    zombie->getAnimator()->clearFrameQueue();
    zombie->setRunStats(false);
}


bool Zombie3RunLoop::onMessage(HumanBase* zombie, const Telegram& msg)
{
    
    return false;
}


//
// Zombie3Attack1
//
void Zombie3Attack1::enter(HumanBase* zombie)
{
    zombie->getAnimator()->pushAnimationFrames(&AnimZombie3Attack1::getInstance());
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


void Zombie3Attack1::execute(HumanBase* zombie)
{
    int inputMask = zombie->getInputMask();
    Vec2 moving = zombie->getMoving();
    
    if ( zombie->getAnimator()->isQueueEmpty() )
    {
        zombie->getFSM()->changeState(&Zombie3IdleLoop::getInstance());
    }
    
    if( isMasked(inputMask, (int)HumanBehaviorType::MOVE) ) zombie->setVelocity( moving * zombie->getWalkSpeed() );
    else zombie->setVelocity( Vec2::ZERO );
}


void Zombie3Attack1::exit(HumanBase* zombie)
{
    zombie->getAnimator()->clearFrameQueue();
}


bool Zombie3Attack1::onMessage(HumanBase* zombie, const Telegram& msg)
{
    return false;
}


//
// Zombie3Attack2
//
void Zombie3Attack2::enter(HumanBase* zombie)
{
    zombie->getAnimator()->pushAnimationFrames(&AnimZombie3Attack2::getInstance());
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


void Zombie3Attack2::execute(HumanBase* zombie)
{
    int inputMask = zombie->getInputMask();
    Vec2 moving = zombie->getMoving();
    
    if ( zombie->getAnimator()->isQueueEmpty() )
    {
        zombie->getFSM()->changeState(&Zombie3IdleLoop::getInstance());
    }
    
    if( isMasked(inputMask, (int)HumanBehaviorType::MOVE) ) zombie->setVelocity( moving * zombie->getWalkSpeed() );
    else zombie->setVelocity( Vec2::ZERO );
}


void Zombie3Attack2::exit(HumanBase* zombie)
{
    zombie->getAnimator()->clearFrameQueue();
}


bool Zombie3Attack2::onMessage(HumanBase* zombie, const Telegram& msg)
{
    return false;
}


//
// Zombie3Dead
//
void Zombie3Dead::enter(HumanBase* zombie)
{
    zombie->setLocalZOrder(Z_ORDER_HUMAN - 1);
    zombie->getGame()->sendMessage(cocos2d::random(3.0, 7.0), zombie, zombie, MessageType::DIE, nullptr);
    zombie->setDead();
    
    zombie->getAnimator()->setShadowVisible(false);
    
    zombie->getAnimator()->pushAnimationFrames(&AnimZombie3Dead::getInstance());
    zombie->setStateName("dead");
    
    SoundSource s;
    s.fileName = std::string("ZombieDie") + _to_string(cocos2d::random(0, 2)) + ".mp3";
    s.position = zombie->getWorldPosition();
    s.soundRange = 800.0f;
    zombie->getGame()->pushLogic(0.0, MessageType::PLAY_SOUND, &s);
}


void Zombie3Dead::execute(HumanBase* zombie)
{
    
}


void Zombie3Dead::exit(HumanBase* zombie)
{
    zombie->getAnimator()->clearFrameQueue();
}


bool Zombie3Dead::onMessage(HumanBase* zombie, const Telegram& msg)
{
    if ( msg.msg == MessageType::DIE )
    {
        //        zombie->getGame()->removeEntity(Zombie3);
        
        return true;
    }
    
    return false;
}









