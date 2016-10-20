//
//  EntityZombie3.cpp
//  DeadCreator
//
//  Created by mac on 2016. 10. 4..
//
//

#include "EntityZombie3.hpp"


#include "EntityZombie3.hpp"
#include "Zombie3States.hpp"
#include "StringHelper.hpp"
#include "Game.hpp"
#include "AnimatedFiniteEntity.hpp"
#include "ZombieBrain.hpp"
#include "UiLayer.hpp"
using namespace realtrick::client;
using namespace cocos2d;


EntityZombie3::EntityZombie3(Game* game) : HumanBase(game)
{
    setEntityType(EntityType::ENTITY_ZOMBIE3);
    setRunSpeed(150.0f);
    setWalkSpeed(75.0f);
    _maxBlood = 100;
    _blood = _maxBlood;
}


EntityZombie3::~EntityZombie3()
{
    CC_SAFE_DELETE(_FSM);
    CC_SAFE_DELETE(_brain);
}


bool EntityZombie3::init()
{
    if ( !HumanBase::init() )
        return false;
    
    _FSM = new StateMachine(this);
    _FSM->setCurrState(&Zombie3IdleLoop::getInstance());
    _FSM->changeState(&Zombie3IdleLoop::getInstance());
    
    return true;
}


EntityZombie3* EntityZombie3::create(Game* game)
{
    auto ret = new (std::nothrow) EntityZombie3(game);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


void EntityZombie3::update(float dt)
{
    HumanBase::update(dt);
}


bool EntityZombie3::handleMessage(const Telegram& msg)
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
            
            this->getFSM()->changeState(&Zombie3Dead::getInstance());
        }
        
        // only apply to player
        if ( _uiLayer )
        {
            float h = _blood / static_cast<float>(_maxBlood);
            h = cocos2d::clampf(h, 0.0f, 1.0f);
            _uiLayer->setHitPoint(h);
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


void EntityZombie3::suicide()
{
    if ( _FSM ) _FSM->changeState(&Zombie3Dead::getInstance());
}




