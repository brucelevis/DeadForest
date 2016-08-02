//
//  EntityHuman.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 15..
//
//

#include "EntityHuman.hpp"
#include "HumanOwnedAnimations.hpp"
#include "HumanOwnedStates.hpp"
#include "ParamLoader.hpp"
#include "Game.hpp"
#include "ItemGlock17.hpp"
#include "Items.hpp"
#include "Inventory.hpp"
#include "WeaponStatus.hpp"
using namespace cocos2d;
using namespace realtrick::client;

EntityHuman::EntityHuman(Game* game) : DynamicEntity(game),
_FSM(nullptr),
_bodyAnimationPlayer(nullptr),
_equipedWeapon(nullptr),
_inputMask(0),
_blood(0),
_maxBlood(0),
_isAlive(false),
_isRun(false),
_isFovOn(false),
_walkSpeed(Prm.getValueAsFloat("walkSpeed")),
_runSpeed(Prm.getValueAsFloat("runSpeed")),
_footGauge(0.0f),
_bodyRot(0.0f),
_inventory(nullptr),
_weaponStatus(nullptr),
_userNickName(""),
_stateName("idle")
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
    
    _inventory = Inventory::create(_game);
    _inventory->retain();
    
    _weaponStatus = WeaponStatus::create(_game);
    _weaponStatus->retain();
    
    return true;
}


EntityHuman* EntityHuman::create(Game* game)
{
    EntityHuman* ret = new (std::nothrow) EntityHuman(game);
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
    // move and rotate
    this->moveEntity();
    this->rotateEntity();
    
    // calculate foot guage to foot step sound.
    this->setFootGauge( _footGauge + _speed * dt );
    
    // 1. update finite state machine
    if ( _FSM ) _FSM->update(dt);
    
    // 2. update animation
    if ( _bodyAnimationPlayer ) _bodyAnimationPlayer->processAnimation(dt);
}


bool EntityHuman::isIntersectOther(const cocos2d::Vec2& futurePosition, EntityBase* other)
{
    if ( other->getEntityType() == ENTITY_HUMAN )
    {
        EntityHuman* human = static_cast<EntityHuman*>(other);
        if( human->isAlive() && physics::intersect(Circle(futurePosition, getBoundingRadius()),
                                                   Circle(human->getWorldPosition(), other->getBoundingRadius())) )
        {
            return true;
        }
    }
    else if ( isMasked(other->getFamilyMask(), ITEM_BASE) )
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


void EntityHuman::setFootGauge(float g)
{
    if ( g > 50.0f )
    {
        _footGauge = 0.0f;
        
        TileType onTile = _game->getStepOnTileType(getWorldPosition());
        
        SoundSource s;
        s.position = getWorldPosition();
        s.soundRange = 50.0f;
        s.volume = 0.2f;
        
        if ( onTile == TileType::DIRT ) s.fileName = "Dirt" + _to_string(random(1, 4)) + ".mp3";
        else if ( onTile == TileType::GRASS ) s.fileName = "Grass" + _to_string(random(1, 4)) + ".mp3";
        else if ( onTile == TileType::WATER ) s.fileName = "Water" + _to_string(random(1, 4)) + ".mp3";
        
        _game->sendMessage(0.0, this, this, MessageType::PLAY_SOUND, &s);
        
        return ;
    }
    
    _footGauge = g;
}









