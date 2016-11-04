//
//  EntityPlayer.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 8. 2..
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


EntityPlayer::EntityPlayer(Game* game) : HumanBase(game)
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


bool EntityPlayer::isIntersectOther(const cocos2d::Vec2& futurePosition, EntityBase* other, cocos2d::Vec2& additionalVelocity)
{
    bool ret = HumanBase::isIntersectOther(futurePosition, other, additionalVelocity);
    
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









