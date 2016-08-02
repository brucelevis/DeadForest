//
//  EntityZombie.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 2..
//
//

#include "EntityZombie.hpp"
using namespace realtrick::client;

EntityZombie::EntityZombie(Game* game) : HumanBase(game),
_FSM(nullptr)
{
}


EntityZombie::~EntityZombie()
{
}


bool EntityZombie::init()
{
    if ( !HumanBase::init() )
        return false;
    
    
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









