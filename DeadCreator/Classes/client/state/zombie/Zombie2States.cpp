//
//  Zombie2States.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 10. 4..
//
//

#include "Zombie2States.hpp"
#include "HumanBase.hpp"
#include "Animator.hpp"
#include "Animations.hpp"
#include "Game.hpp"
using namespace realtrick::client;
using namespace cocos2d;


//
// Zombie2IdleLoop
//
void Zombie2IdleLoop::enter(HumanBase* zombie)
{
    zombie->getAnimator()->pushAnimationFrames(&AnimZombie2IdleLoop::getInstance());
    zombie->setVelocity( Vec2::ZERO );
    zombie->setStateName("idle");
}


void Zombie2IdleLoop::execute(HumanBase* zombie)
{
    int inputMask = zombie->getInputMask();
    Vec2 moving = zombie->getMoving();
    
    if ( zombie->getAnimator()->isQueueEmpty() )
    {
        zombie->getAnimator()->pushAnimationFrames(&AnimZombie2IdleLoop::getInstance());
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::ATTACK) )
    {
        int random = cocos2d::random(0, 1);
        if ( random == 0 ) zombie->getFSM()->changeState(&Zombie2Attack1::getInstance());
        else zombie->getFSM()->changeState(&Zombie2Attack2::getInstance());
    }
    
    if ( isMasked(inputMask, HumanBehaviorType::TURN) )
    {
        zombie->setVelocity( moving * zombie->getWalkSpeed() );
    }
    
    if ( inputMask == HumanBehaviorType::MOVE )
    {
        zombie->getFSM()->changeState(&Zombie2RunLoop::getInstance());
    }
    
    if ( !isMasked(inputMask, HumanBehaviorType::MOVE) )
    {
        zombie->setVelocity( Vec2::ZERO );
    }
}


void Zombie2IdleLoop::exit(HumanBase* zombie)
{
    zombie->getAnimator()->clearFrameQueue();
}


bool Zombie2IdleLoop::onMessage(HumanBase* zombie, const Telegram& msg)
{
    return false;
}


//
// Zombie2RunLoop
//
void Zombie2RunLoop::enter(HumanBase* zombie)
{
    zombie->getAnimator()->pushFramesAtoB(&AnimZombie2RunLoop::getInstance(), 0, 6);
    zombie->setRunStats(true);
    zombie->setStateName("run");
}


void Zombie2RunLoop::execute(HumanBase* zombie)
{
    int inputMask = zombie->getInputMask();
    Vec2 moving = zombie->getMoving();
    int currFrame = zombie->getAnimator()->getFrameIndex();
    
    if( zombie->getAnimator()->isQueueEmpty() )
    {
        zombie->getFSM()->changeState(&Zombie2IdleLoop::getInstance());
    }
    
    if ( inputMask == HumanBehaviorType::MOVE )
    {
        zombie->setVelocity( moving * zombie->getRunSpeed() );
        if( moving != Vec2::ZERO ) zombie->setTargetHeading(moving);
        
        if ( currFrame == 6 )
        {
            zombie->getAnimator()->pushFramesAtoB(&AnimZombie2RunLoop::getInstance(), 7, 11);
        }
        else if( currFrame == 11 )
        {
            zombie->getAnimator()->pushFramesAtoB(&AnimZombie2RunLoop::getInstance(), 0, 6);
        }
    }
}


void Zombie2RunLoop::exit(HumanBase* zombie)
{
    zombie->getAnimator()->clearFrameQueue();
    zombie->setRunStats(false);
}


bool Zombie2RunLoop::onMessage(HumanBase* zombie, const Telegram& msg)
{
    
    return false;
}


//
// Zombie2Attack1
//
void Zombie2Attack1::enter(HumanBase* zombie)
{
    zombie->getAnimator()->pushAnimationFrames(&AnimZombie2Attack1::getInstance());
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


void Zombie2Attack1::execute(HumanBase* zombie)
{
    int inputMask = zombie->getInputMask();
    Vec2 moving = zombie->getMoving();
    
    if ( zombie->getAnimator()->isQueueEmpty() )
    {
        zombie->getFSM()->changeState(&Zombie2IdleLoop::getInstance());
    }
    
    if( isMasked(inputMask, (int)HumanBehaviorType::MOVE) ) zombie->setVelocity( moving * zombie->getWalkSpeed() );
    else zombie->setVelocity( Vec2::ZERO );
}


void Zombie2Attack1::exit(HumanBase* zombie)
{
    zombie->getAnimator()->clearFrameQueue();
}


bool Zombie2Attack1::onMessage(HumanBase* zombie, const Telegram& msg)
{
    return false;
}


//
// Zombie2Attack2
//
void Zombie2Attack2::enter(HumanBase* zombie)
{
    zombie->getAnimator()->pushAnimationFrames(&AnimZombie2Attack2::getInstance());
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


void Zombie2Attack2::execute(HumanBase* zombie)
{
    int inputMask = zombie->getInputMask();
    Vec2 moving = zombie->getMoving();
    
    if ( zombie->getAnimator()->isQueueEmpty() )
    {
        zombie->getFSM()->changeState(&Zombie2IdleLoop::getInstance());
    }
    
    if( isMasked(inputMask, (int)HumanBehaviorType::MOVE) ) zombie->setVelocity( moving * zombie->getWalkSpeed() );
    else zombie->setVelocity( Vec2::ZERO );
}


void Zombie2Attack2::exit(HumanBase* zombie)
{
    zombie->getAnimator()->clearFrameQueue();
}


bool Zombie2Attack2::onMessage(HumanBase* zombie, const Telegram& msg)
{
    return false;
}


//
// Zombie2Dead
//
void Zombie2Dead::enter(HumanBase* zombie)
{
    zombie->setLocalZOrder(Z_ORDER_HUMAN - 1);
    zombie->getGame()->sendMessage(cocos2d::random(3.0, 7.0), zombie, zombie, MessageType::DIE, nullptr);
    zombie->setDead();
    
    zombie->getAnimator()->setShadowVisible(false);
    
    zombie->getAnimator()->pushAnimationFrames(&AnimZombie2Dead::getInstance());
    zombie->setStateName("dead");
    
    SoundSource s;
    s.fileName = std::string("ZombieDie") + _to_string(cocos2d::random(0, 2)) + ".mp3";
    s.position = zombie->getWorldPosition();
    s.soundRange = 800.0f;
    zombie->getGame()->pushLogic(0.0, MessageType::PLAY_SOUND, &s);
}


void Zombie2Dead::execute(HumanBase* zombie)
{
    
}


void Zombie2Dead::exit(HumanBase* zombie)
{
    zombie->getAnimator()->clearFrameQueue();
}


bool Zombie2Dead::onMessage(HumanBase* zombie, const Telegram& msg)
{
    if ( msg.msg == MessageType::DIE )
    {
        //        zombie->getGame()->removeEntity(Zombie2);
        
        return true;
    }
    
    return false;
}









