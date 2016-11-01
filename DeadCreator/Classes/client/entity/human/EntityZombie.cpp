//
//  EntityZombie.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 8. 2..
//
//

#include "EntityZombie.hpp"
#include "ZombieStates.hpp"
#include "StringHelper.hpp"
#include "Game.hpp"
#include "AnimatedFiniteEntity.hpp"
#include "ZombieBrain.hpp"
#include "UiLayer.hpp"
using namespace realtrick::client;
using namespace cocos2d;


EntityZombie::EntityZombie(Game* game) : HumanBase(game)
{
    setEntityType(EntityType::ENTITY_ZOMBIE);
    setRunSpeed(150.0f);
    setWalkSpeed(75.0f);
    _maxBlood = 120;
    _blood = _maxBlood;
}


EntityZombie::~EntityZombie()
{
    CC_SAFE_DELETE(_FSM);
    CC_SAFE_DELETE(_brain);
}


bool EntityZombie::init()
{
    if ( !HumanBase::init() )
        return false;
    
    _FSM = new StateMachine(this);
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


void EntityZombie::suicide()
{
    if ( _FSM ) _FSM->changeState(&ZombieDead::getInstance());
}









