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
#include "AnimatedFiniteEntity.hpp"
#include "RenderingSystem.hpp"
#include "ZombieBrain.hpp"
using namespace cocos2d;
using namespace realtrick::client;


EntityPlayer::EntityPlayer(Game* game) : HumanBase(game),
_equipedWeapon(nullptr),
_weaponStatus(nullptr),
_inventory(nullptr)
{
    setEntityType(EntityType::ENTITY_PLAYER);
    setMaxSpeed(100.0f);
}


EntityPlayer::~EntityPlayer()
{
    CC_SAFE_DELETE(_FSM);
    CC_SAFE_RELEASE_NULL(_weaponStatus);
    CC_SAFE_RELEASE_NULL(_inventory);
    
    CC_SAFE_DELETE(_brain);
}


bool EntityPlayer::init()
{
    if ( !HumanBase::init() )
        return false;
    
    _FSM = new StateMachine(this);
    _FSM->setCurrState(&HumanFistIdleLoop::getInstance());
    _FSM->changeState(&HumanFistIdleLoop::getInstance());
    
    _weaponStatus = WeaponStatus::create(_game);
    _weaponStatus->retain();
    
    _inventory = Inventory::create(_game);
    _inventory->retain();
    
//    _brain = new ZombieBrain(this);
    
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


bool EntityPlayer::handleMessage(const realtrick::client::Telegram &msg)
{
    bool ret = false;
    
    ret = HumanBase::handleMessage(msg);
    
    if ( msg.msg == MessageType::WEAPON_READY )
    {
        WeaponBase* weapon = static_cast<WeaponBase*>(msg.extraInfo);
        weapon->enableReadyToAttack(true);
        
        ret = true;
    }
    
    else if ( msg.msg  == MessageType::HITTED_BY_GUN )
    {
        ReceiverSenderDamage s = *static_cast<ReceiverSenderDamage*>(msg.extraInfo);
        if ( _blood > 0 ) _blood -= s.damage;
        if ( _blood <= 0 && isAlive() ) this->getFSM()->changeState(&HumanBackDeadState::getInstance());
        
        AnimatedFiniteEntity* blood = AnimatedFiniteEntity::create(_game, {"blood" + _to_string(random(1, 5)) + ".png"},
                                                                   random(5.0f, 10.0f), cocos2d::ui::Widget::TextureResType::PLIST);
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
        if ( _blood <= 0 && isAlive() ) this->getFSM()->changeState(&HumanBackDeadState::getInstance());
        
        AnimatedFiniteEntity* blood = AnimatedFiniteEntity::create(_game, {"big_blood.PNG"},
                                                                   random(5.0f, 10.0f), cocos2d::ui::Widget::TextureResType::PLIST);
        blood->setScale(0.5f);
        blood->setWorldPosition(getWorldPosition());
        blood->setRotation(random(0.0f, 360.f));
        _game->addEntity(blood);
        
        ret = true;
    }
    
    return ret;
}


void EntityPlayer::suicide()
{
    if ( _FSM ) _FSM->changeState(&HumanBackDeadState::getInstance());
}








