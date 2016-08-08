//
//  EntityZombie.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 2..
//
//

#include "EntityZombie.hpp"
#include "ZombieStates.hpp"
#include "StringHelper.hpp"
#include "Game.hpp"
#include "AnimatedFiniteEntity.hpp"
using namespace realtrick::client;
using namespace cocos2d;


EntityZombie::EntityZombie(Game* game) : HumanBase(game),
_FSM(nullptr)
{
    setEntityType(EntityType::ENTITY_ZOMBIE);
}


EntityZombie::~EntityZombie()
{
    CC_SAFE_DELETE(_FSM);
}


bool EntityZombie::init()
{
    if ( !HumanBase::init() )
        return false;
    
    _FSM = new StateMachine<EntityZombie>(this);
    _FSM->setCurrState(&ZombieIdleLoop::getInstance());
    _FSM->changeState(&ZombieIdleLoop::getInstance());
    
    return true;
}


EntityZombie* EntityZombie::create(Game* game)
{
    auto ret = new (std::nothrow) EntityZombie(game);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


void EntityZombie::update(float dt)
{
    HumanBase::update(dt);
    
    if ( _FSM ) _FSM->update(dt);
}


bool EntityZombie::handleMessage(const Telegram& msg)
{
    bool ret = _FSM->handleMessage(msg);
    
    if ( msg.msg  == MessageType::HITTED_BY_GUN )
    {
        ReceiverSenderDamage s = *static_cast<ReceiverSenderDamage*>(msg.extraInfo);
        if ( _blood > 0 ) _blood -= s.damage;
        if ( _blood <= 0 && isAlive() ) this->getFSM()->changeState(&ZombieDead::getInstance());
        
        AnimatedFiniteEntity* blood = AnimatedFiniteEntity::create(_game, {"blood" + _to_string(random(1, 5)) + ".png"},
                                                                   random(5, 10), cocos2d::ui::Widget::TextureResType::PLIST);
        blood->setWorldPosition(Vec2(getWorldPosition().x + random(-30, 30),
                                     getWorldPosition().y + random(-30, 30)));
        blood->setScale(0.2f);
        _game->addEntity(blood);
        
        ret = true;
    }
    
    else if (msg.msg == MessageType::HITTED_BY_AXE )
    {
        ReceiverSenderDamage s = *static_cast<ReceiverSenderDamage*>(msg.extraInfo);
        if ( _blood > 0 ) _blood -= s.damage;
        if ( _blood <= 0 && isAlive() ) this->getFSM()->changeState(&ZombieDead::getInstance());
        
        
        AnimatedFiniteEntity* blood = AnimatedFiniteEntity::create(_game, {"big_blood.PNG"},
                                                                   random(5.0f, 10.0f), cocos2d::ui::Widget::TextureResType::PLIST);
        blood->setRotation(random(0.0f, 360.f));
        blood->setWorldPosition(getWorldPosition());
        blood->setScale(0.5f);
        _game->addEntity(blood);
        
        ret = true;
    }
    
    return ret;
}


void EntityZombie::suicide()
{
    _FSM->changeState(&ZombieDead::getInstance());
}








