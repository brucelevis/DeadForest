//
//  EntityZombie2.cpp
//  DeadCreator
//
//  Created by mac on 2016. 10. 4..
//
//

#include "EntityZombie2.hpp"
#include "Zombie2States.hpp"
#include "StringHelper.hpp"
#include "Game.hpp"
#include "AnimatedFiniteEntity.hpp"
#include "ZombieBrain.hpp"
#include "UiLayer.hpp"
using namespace realtrick::client;
using namespace cocos2d;


EntityZombie2::EntityZombie2(Game* game) : HumanBase(game)
{
    setEntityType(EntityType::ENTITY_ZOMBIE2);
    setRunSpeed(150.0f);
    setWalkSpeed(75.0f);
    _maxBlood = 400;
    _blood = _maxBlood;
}


EntityZombie2::~EntityZombie2()
{
    CC_SAFE_DELETE(_FSM);
    CC_SAFE_DELETE(_brain);
}


bool EntityZombie2::init()
{
    if ( !HumanBase::init() )
        return false;
    
    _FSM = new StateMachine(this);
    _FSM->setCurrState(&Zombie2IdleLoop::getInstance());
    _FSM->changeState(&Zombie2IdleLoop::getInstance());
    
    return true;
}


EntityZombie2* EntityZombie2::create(Game* game)
{
    auto ret = new (std::nothrow) EntityZombie2(game);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


void EntityZombie2::update(float dt)
{
    HumanBase::update(dt);
}


bool EntityZombie2::handleMessage(const Telegram& msg)
{
    bool ret = false;
    
    ret = HumanBase::handleMessage(msg);
    
    if ( msg.msg == MessageType::HITTED_BY_GUN || msg.msg == MessageType::HITTED_BY_AXE || msg.msg == MessageType::HITTED_BY_FIST )
    {
        ReceiverSenderDamage* d = static_cast<ReceiverSenderDamage*>(msg.extraInfo);
        if ( _blood > 0 ) _blood -= d->damage;
        if ( _blood <= 0 && isAlive() )
        {
            if ( d->sender->getTag() == _game->getPlayerPtr()->getTag() )
            {
                if ( msg.msg == MessageType::HITTED_BY_GUN)
                {
                    SoundSource s;
                    s.fileName = "kill_sound.mp3";
                    s.position = static_cast<HumanBase*>(d->sender)->getWorldPosition();
                    s.soundRange = 1000.0f;
                    s.volume = 1.0f;
                    _game->pushLogic(0.0, MessageType::PLAY_SOUND, &s);
                }
            }
            
            this->getFSM()->changeState(&Zombie2Dead::getInstance());
        }
        
        ret = true;
    }
    
    else if ( msg.msg == MessageType::HIT )
    {
        // only apply to player
        if ( _uiLayer ) _uiLayer->runCrossHairEffect("hit");
        
        ret = true;
    }
    
    else if ( msg.msg == MessageType::NO_HIT )
    {
        // only apply to player
        if ( _uiLayer ) _uiLayer->runCrossHairEffect("fire");
        
        ret = true;
    }
    
    return ret;
}


void EntityZombie2::suicide()
{
    if ( _FSM ) _FSM->changeState(&Zombie2Dead::getInstance());
}









