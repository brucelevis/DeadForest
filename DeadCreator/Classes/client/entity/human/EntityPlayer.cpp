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
using namespace cocos2d;
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
    HumanBase::update(dt);
    
    if ( _FSM ) _FSM->update(dt);
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
    bool ret = _FSM->handleMessage(msg);
    
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
        
        for(int i = 0 ; i < 5 ; ++ i)
        {
            AnimatedFiniteEntity* blood = AnimatedFiniteEntity::create(_game, {"blood1.png"},
                                                                       random(5.0f, 10.0f), cocos2d::ui::Widget::TextureResType::PLIST);
            blood->setWorldPosition(Vec2(getWorldPosition().x + random(-20, 20),
                                         getWorldPosition().y + random(-20, 20)));
            blood->setScale(0.3f);
            _game->addEntity(blood);
        }
        
        ret = true;
    }
    else if ( msg.msg == MessageType::PLAY_SOUND )
    {
        SoundSource s =  *static_cast<SoundSource*>(msg.extraInfo);
        float t = (1.0f - (s.position - _game->getRenderingSysetm()->getCameraPosition()).getLength() / s.soundRange) * s.volume;
        experimental::AudioEngine::setVolume( experimental::AudioEngine::play2d(s.fileName), t);
        
        ret = true;
    }
    
    return ret;
}


void EntityPlayer::suicide()
{
    _FSM->changeState(&HumanBackDeadState::getInstance());
}








