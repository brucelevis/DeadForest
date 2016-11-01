//
//  EntityZombie2.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 10. 4..
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


void EntityZombie2::suicide()
{
    if ( _FSM ) _FSM->changeState(&Zombie2Dead::getInstance());
}









