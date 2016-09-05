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
#include "AnimatedFiniteEntity.hpp"
#include "RenderingSystem.hpp"
#include "PursuerBrain.hpp"
#include "InventoryData.hpp"
#include "UiLayer.hpp"
using namespace cocos2d;
using namespace realtrick::client;


EntityPlayer::EntityPlayer(Game* game) : HumanBase(game),
_uiLayer(nullptr),
_equipedWeapon(nullptr),
_userNickName("")
{
    setEntityType(EntityType::ENTITY_PLAYER);
    setRunSpeed(150.0f);
    setWalkSpeed(75.0f);
    _maxBlood = 100;
    _blood = _maxBlood;
}


EntityPlayer::~EntityPlayer()
{
    CC_SAFE_DELETE(_FSM);
    CC_SAFE_DELETE(_inventoryData);
}


bool EntityPlayer::init()
{
    if ( !HumanBase::init() )
        return false;
    
    _FSM = new StateMachine(this);
    _FSM->setCurrState(&HumanFistIdleLoop::getInstance());
    _FSM->changeState(&HumanFistIdleLoop::getInstance());
    
    _inventoryData = new InventoryData(this);
    
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
    
    if ( msg.msg == MessageType::HITTED_BY_GUN || msg.msg == MessageType::HITTED_BY_AXE )
    {
        ReceiverSenderDamage* s = static_cast<ReceiverSenderDamage*>(msg.extraInfo);
        if ( _blood > 0 ) _blood -= s->damage;
        if ( _blood <= 0 && isAlive() ) this->getFSM()->changeState(&HumanBackDeadState::getInstance());
        
        // only apply to player
        if ( _uiLayer )
        {
            float h = _blood / static_cast<float>(_maxBlood);
            h = cocos2d::clampf(h, 0.0f, 1.0f);
            _uiLayer->setHitPoint(h);
        }
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
    
    else if ( msg.msg == MessageType::DISPLAY_TEXT )
    {
        // only apply to player
        if ( _uiLayer )
        {
            auto text = *static_cast<std::string*>(msg.extraInfo);
            _uiLayer->displayText(text);
        }
        
        ret = true;
    }
    
    return ret;
}


void EntityPlayer::suicide()
{
    if ( _FSM ) _FSM->changeState(&HumanBackDeadState::getInstance());
}


void EntityPlayer::reload()
{
    if ( _equipedWeapon ) _equipedWeapon->reload();
}


bool EntityPlayer::addItem(ItemBase* item)
{
    return _inventoryData->addItem(item);
}








