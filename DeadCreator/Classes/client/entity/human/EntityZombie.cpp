//
//  EntityZombie.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 2..
//
//

#include "EntityZombie.hpp"
#include "ZombieStates.hpp"
using namespace realtrick::client;

EntityZombie::EntityZombie(Game* game) : HumanBase(game),
_FSM(nullptr)
{
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









