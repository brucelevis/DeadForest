//
//  EntityPlayer.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 2..
//
//

#include "EntityPlayer.hpp"
#include "HumanOwnedAnimations.hpp"
#include "HumanOwnedStates.hpp"
#include "Game.hpp"
#include "Inventory.hpp"
#include "WeaponStatus.hpp"
using namespace realtrick::client;


EntityPlayer::EntityPlayer(Game* game) : HumanBase(game),
_FSM(nullptr),
_equipedWeapon(nullptr),
_weaponStatus(nullptr),
_inventory(nullptr)
{
    setEntityType(EntityType::ENTITY_PLAYER);
}


EntityPlayer::~EntityPlayer()
{
    CC_SAFE_DELETE(_FSM);
    
    CC_SAFE_RELEASE_NULL(_weaponStatus);
    CC_SAFE_RELEASE_NULL(_inventory);
}


bool EntityPlayer::init()
{
    if ( !HumanBase::init() )
        return false;
    
    _FSM = new StateMachine<EntityPlayer>(this);
    _FSM->setGlobalState(&HumanGlobalState::getInstance());
    _FSM->setCurrState(&HumanFistIdleLoop::getInstance());
    _FSM->changeState(&HumanFistIdleLoop::getInstance());

    _weaponStatus = WeaponStatus::create(_game);
    _weaponStatus->retain();
    
    _inventory = Inventory::create(_game);
    _inventory->retain();
    
    return true;
}


EntityPlayer* EntityPlayer::create(Game* game)
{
    auto ret = new (std::nothrow) EntityPlayer(game);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


void EntityPlayer::update(float dt)
{
    if ( _FSM ) _FSM->update(dt);
    
    HumanBase::update(dt);
}


bool EntityPlayer::isIntersectOther(const cocos2d::Vec2& futurePosition, EntityBase* other)
{
    bool ret = HumanBase::isIntersectOther(futurePosition, other);;
    
    if ( isMasked(other->getFamilyMask(), ITEM_BASE) )
    {
        if ( physics::intersect(Circle(futurePosition, getBoundingRadius()),
                                Circle(other->getWorldPosition(), other->getBoundingRadius())))
        {
            ItemAndOwner data;
            data.owner = this;
            data.item = static_cast<ItemBase*>(other);
            _game->pushLogic(0.0, MessageType::PUSH_ITEM_TO_INVENTORY, &data);
            _game->removeEntity(other);
        }
    }
    
    return ret;
}









