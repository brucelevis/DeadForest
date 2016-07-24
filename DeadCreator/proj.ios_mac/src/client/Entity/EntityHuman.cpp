//
//  EntityHuman.cpp
//  TheDeadForest
//
//  Created by 남준현 on 2015. 11. 15..
//
//

#include "EntityHuman.hpp"
#include "HumanOwnedAnimations.hpp"
#include "HumanOwnedStates.hpp"
#include "ParamLoader.hpp"
#include "GameManager.hpp"
#include "ItemGlock17.hpp"
#include "GameWorld.hpp"
#include "Items.hpp"
#include "AnimatedButton.hpp"
#include "Inventory.hpp"
#include "WeaponStatus.hpp"
#include "UiLayer.hpp"
using namespace cocos2d;
using namespace realtrick;

EntityHuman::EntityHuman(GameManager* mgr) : DynamicEntity(mgr),
_FSM(nullptr),
_bodyAnimationPlayer(nullptr),
_isWeaponEquipCompletly(false),
_equipedWeapon(nullptr),
_inputMask(0),
_blood(0),
_maxBlood(0),
_isAlive(false),
_isFovOn(false),
_bodyRot(0.0f),
_walkSpeed(Prm.getValueAsFloat("walkSpeed")),
_runSpeed(Prm.getValueAsFloat("runSpeed")),
_noEventTime(0.0f),
_enduranceTime(0.0f),
_isRun(false),
_isAiming(false),
_footGauge(0.0f),
_userNickName("")
{
    setEntityType(ENTITY_HUMAN);
    setBoundingRadius(Prm.getValueAsFloat("boundingRadius"));
    setTurnSpeed(Prm.getValueAsFloat("turnSpeed"));
    setMaxSpeed(Prm.getValueAsFloat("maxSpeed"));
}


EntityHuman::~EntityHuman()
{
    CC_SAFE_DELETE(_bodyAnimationPlayer);
    CC_SAFE_DELETE(_FSM);
    CC_SAFE_RELEASE(_inventory);
    CC_SAFE_RELEASE(_weaponStatus);
    CC_SAFE_DELETE(_aimingSystem);
}


bool EntityHuman::init()
{
    _bodyAnimationPlayer = new AnimationPlayer(this, &AnimHumanFistIdleLoop::getInstance(), 4);
    
    _FSM = new StateMachine<EntityHuman>(this);
    _FSM->setGlobalState(&HumanGlobalState::getInstance());
    _FSM->setCurrState(&HumanFistIdleLoop::getInstance());
    _FSM->changeState(&HumanFistIdleLoop::getInstance());
    
    _maxBlood = Prm.getValueAsInt("maxBlood");
    _blood = _maxBlood;
    
    setAlive();
    
    _enduranceTime  = 5.0f;
    
    _inventory = userinterface::Inventory::create(_gameMgr);
    _inventory->retain();
    
    _weaponStatus = userinterface::WeaponStatus::create(_gameMgr);
    _weaponStatus->retain();
    
    _aimingSystem = new AimingSystem(_gameMgr, this);
    _aimingSystem->enableSystem(true);
    
    return true;
}


EntityHuman* EntityHuman::create(GameManager* mgr)
{
    EntityHuman* ret = new (std::nothrow) EntityHuman(mgr);
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


void EntityHuman::update(float dt)
{
    moveEntity();
    rotateEntity();
    
    setFootGauge( getFootGauge() + _speed * dt );
    
    if ( _FSM ) _FSM->update(dt);
    if ( _bodyAnimationPlayer ) _bodyAnimationPlayer->processAnimation(dt);
}


void EntityHuman::setDead()
{
    _isAlive = false;
}


void EntityHuman::setAlive()
{
    _isAlive = true;
    _blood = _maxBlood;
}


bool EntityHuman::handleMessage(const Telegram& msg)
{
    return _FSM->handleMessage(msg);
}


bool EntityHuman::isIntersectOther(const cocos2d::Vec2& futurePosition, EntityBase* other)
{
    if ( other->getEntityType() == ENTITY_HUMAN )
    {
        EntityHuman* human = static_cast<EntityHuman*>(other);
        if( human->isAlive() && physics::intersect(Circle(futurePosition, getBoundingRadius()), Circle(human->getWorldPosition(), 20.0f)) )
        {
            return true;
        }
    }
    else if ( isMasked(other->getFamilyMask(), ITEM_BASE) )
    {
        if ( physics::intersect(Circle(futurePosition, getBoundingRadius()), Circle(other->getWorldPosition(), 20.0f)))
        {
            ItemAndOwner data;
            data.owner = this;
            data.item = static_cast<ItemBase*>(other);
            _gameMgr->pushLogic(0.0, MessageType::PUSH_ITEM_TO_INVENTORY, &data);
            _gameMgr->removeEntity(other->getTag());
        }
    }
    
    return false;
}


bool EntityHuman::isIntersectWall(const cocos2d::Vec2& futurePosition, const Polygon& wall)
{
    for( int i = 0 ; i < wall.vertices.size() - 1 ; ++ i)
    {
        if ( physics::intersect(Circle(futurePosition, getBoundingRadius()), Segment(wall.vertices[i], wall.vertices[i + 1])) )
        {
            return true;
        }
    }
    if ( physics::intersect(Circle(futurePosition, getBoundingRadius()), Segment(wall.vertices.back(), wall.vertices.front())) )
    {
        return true;
    }
    
    return false;
}


void EntityHuman::rotateEntity()
{
    if ( _heading.dot(_targetHeading) < 0.995f )
    {
        float dt = Director::getInstance()->getDeltaTime();
        
        float d = getHeading().cross(_targetHeading);
        if( d > 0 )
        {
            Mat3 rotMat;
            rotMat.rotate(MATH_DEG_TO_RAD(_turnSpeed * dt));
            setHeading(rotMat.getTransformedVector(getHeading()));
        }
        else
        {
            Mat3 rotMat;
            rotMat.rotate(-MATH_DEG_TO_RAD(_turnSpeed * dt));
            setHeading(rotMat.getTransformedVector(getHeading()));
        }
    }
    
    setBodyRot(-physics::getAngleFromZero(getHeading()));
}


void EntityHuman::enableNormal(bool enable)
{
    getBodyAnimator()->enableNormal(enable);
}


void EntityHuman::disableShadow()
{
    getBodyAnimator()->enableShadow(false);
}


void EntityHuman::setFootGauge(float g)
{
    if ( g > 50.0f )
    {
        GameMap* map = _gameMgr->getGameMap();
        GameMap::TileType onTile = map->getStepOnTileType(getWorldPosition());
        
        if ( onTile == GameMap::TileType::DIRT )
        {
            SoundSource s;
            s.fileName = "Dirt" + GameManager::_to_string(random(1, 4)) + ".mp3";
            s.position = getWorldPosition();
            s.soundRange = 50.0f;
            s.volume = 0.2f;
            Dispatch.pushMessage(0.0, this, this, MessageType::PLAY_SOUND, &s);
        }
        else if ( onTile == GameMap::TileType::GRASS )
        {
            SoundSource s;
            s.fileName = "Grass" + GameManager::_to_string(random(1, 4)) + ".mp3";
            s.position = getWorldPosition();
            s.soundRange = 50.0f;
            s.volume = 0.2f;
            Dispatch.pushMessage(0.0, this, this, MessageType::PLAY_SOUND, &s);
        }
        else if ( onTile == GameMap::TileType::WATER )
        {
            SoundSource s;
            s.fileName = "Water" + GameManager::_to_string(random(1, 4)) + ".mp3";
            s.position = getWorldPosition();
            s.volume = 0.2f;
            s.soundRange = 50.0f;
            Dispatch.pushMessage(0.0, this, this, MessageType::PLAY_SOUND, &s);
        }
        
        _footGauge = 0.0f;
        
        return ;
    }
    
    _footGauge = g;
}












