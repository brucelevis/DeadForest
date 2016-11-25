//
//  EntityZombie3.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 10. 4..
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
    _maxBlood = 200;
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


void EntityZombie3::suicide()
{
    if ( _FSM ) _FSM->changeState(&Zombie3Dead::getInstance());
}




