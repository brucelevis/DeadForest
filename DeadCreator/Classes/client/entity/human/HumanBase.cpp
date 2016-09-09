//
//  HumanBase.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 15..
//
//

#include "HumanBase.hpp"
#include "ParamLoader.hpp"
#include "Game.hpp"
#include "Camera2D.hpp"
#include "RenderingSystem.hpp"
#include "ZombieBrain.hpp"
#include "AnimatedFiniteEntity.hpp"
#include "GameResource.hpp"
#include "PathPlanner.h"
#include "SensoryMemory.h"
#include "AbstTargetingSystem.h"
#include "InventoryData.hpp"
#include "UiLayer.hpp"
using namespace cocos2d;
using namespace realtrick::client;
using namespace realtrick;


HumanBase::HumanBase(Game* game) : EntityBase(game),
_animator(nullptr),
_FSM(nullptr),
_brain(nullptr),
_uiLayer(nullptr),
_equipedWeapon(nullptr),
_inventoryData(nullptr),
_heading(Vec2::UNIT_X),
_targetHeading(Vec2::UNIT_X),
_moving(Vec2::UNIT_X),
_left(Vec2::ZERO),
_right(Vec2::ZERO),
_velocity(Vec2::ZERO),
_turnSpeed(0.0f),
_speed(0.0f),
_inputMask(0),
_blood(0),
_maxBlood(0),
_isAlive(false),
_isRun(false),
_isFovOn(false),
_isInventoryOpened(false),
_walkSpeed(0.0f),
_runSpeed(0.0f),
_footGauge(0.0f),
_rotation(0.0f),
_userNickName(""),
_stateName("idle")
{
    ADD_FAMILY_MASK(_familyMask, HUMAN_BASE);
    setBoundingRadius(Prm.getValueAsFloat("boundingRadius"));
    setTurnSpeed(720.0f);
}


HumanBase::~HumanBase()
{
    CC_SAFE_DELETE(_animator);
    CC_SAFE_DELETE(_inventoryData);
    CC_SAFE_DELETE(_brain);
}


bool HumanBase::init()
{
    if ( !Node::init() )
        return false;
    
    _animator = new Animator(this);
    _inventoryData = new InventoryData(this);
    
    setAlive();

	_pathPlanner = new PathPlanner(*_game->getGraph(), this);
	_sensory = new SensoryMemory(this, 5);
	_target_system = new AbstTargetingSystem(this);

    _balance = Node::create();
    addChild(_balance);

    return true;
}


HumanBase* HumanBase::create(Game* game)
{
    HumanBase* ret = new (std::nothrow) HumanBase(game);
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


void HumanBase::update(float dt)
{
    if ( _brain && isAlive() ) _brain->think();
    if ( _FSM ) _FSM->update(dt);
	
	_sensory->updateVision();
	_target_system->update();
	

    // move and rotate
    this->moveEntity();
    this->rotateEntity();
    
    // calculate foot guage to foot step sound.
    this->setFootGauge( _footGauge + _speed * dt );
    
    // update animation
    if ( _animator )
    {
        _animator->setRotation(_rotation);
        _animator->processAnimation(dt);
    }
}


bool HumanBase::isIntersectOther(const cocos2d::Vec2& futurePosition, EntityBase* other)
{
    if ( isMasked(other->getFamilyMask(), HUMAN_BASE) )
    {
        HumanBase* human = static_cast<HumanBase*>(other);
        if( human->isAlive() && physics::intersect(Circle(futurePosition, getBoundingRadius()),
                                                   Circle(human->getWorldPosition(), other->getBoundingRadius())) )
        {
            return true;
        }
    }
    return false;
}


bool HumanBase::isIntersectWall(const cocos2d::Vec2& futurePosition, const realtrick::Polygon& wall)
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


void HumanBase::moveEntity()
{
    if ( getVelocity() == Vec2::ZERO )
    {
        _speed = 0.0f;
        return ;
    }
    
    float dt = Director::getInstance()->getDeltaTime();
    cocos2d::Vec2 oldPos = getWorldPosition();
    cocos2d::Vec2 futurePosition = getWorldPosition() + getVelocity() * dt;
    _speed = getVelocity().getLength();
    bool intersectResult = false;
    
    // 엔티티들과의 충돌처리
    const std::list<EntityBase*> members = _game->getNeighborsOnMove(oldPos, _speed);
    for ( const auto &entity : members )
    {
        if ( entity == this ) continue;
        
        if ( isIntersectOther(futurePosition, entity) )
            intersectResult = true;
    }
    
    // 벽과의 충돌처리
    const std::vector<realtrick::Polygon> walls = _game->getNeighborWalls(futurePosition, _speed);
    for( const auto& wall : walls )
    {
        if ( isIntersectWall(futurePosition, wall) )
            intersectResult = true;
    }
    
    if ( !intersectResult )
    {
        setWorldPosition(futurePosition);
        _game->getCellSpace()->updateEntity(this, oldPos);
    }
}


void HumanBase::rotateEntity()
{
    if ( _heading.dot(_targetHeading) < 0.982546f )
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
        
        setRotationZ(-physics::getAngleFromZero(getHeading()));
    }
}


void HumanBase::setFootGauge(float g)
{
    if ( g > 50.0f )
    {
        _footGauge = 0.0f;
        
        TileType onTile = _game->getStepOnTileType(getWorldPosition());
        
        SoundSource s;
        s.position = getWorldPosition();
        s.soundRange = 1000.0f;
        s.volume = 0.2f;
        
        if ( onTile == TileType::DIRT || onTile == TileType::HILL ) s.fileName = "Dirt" + _to_string(random(1, 4)) + ".mp3";
        else if ( onTile == TileType::GRASS ) s.fileName = "Grass" + _to_string(random(1, 4)) + ".mp3";
        else if ( onTile == TileType::WATER ) s.fileName = "Water" + _to_string(random(1, 4)) + ".mp3";
        
        _game->sendMessage(0.0, this, this, MessageType::PLAY_SOUND, &s);
        
        return ;
    }
    
    _footGauge = g;
}


bool HumanBase::handleMessage(const Telegram& msg)
{
    bool ret = false;
    
    if ( _FSM ) ret = _FSM->handleMessage(msg);
    
    if ( msg.msg == MessageType::PLAY_SOUND )
    {
        SoundSource* s =  static_cast<SoundSource*>(msg.extraInfo);
        float t = (1.0f - (s->position - _game->getCamera()->getCameraPos()).getLength() / s->soundRange) * s->volume;
        experimental::AudioEngine::setVolume( experimental::AudioEngine::play2d("client/sounds/" + s->fileName), t);
        
        ret = true;
    }
    
    else if ( msg.msg  == MessageType::HITTED_BY_GUN )
    {
        AnimatedFiniteEntity* blood = AnimatedFiniteEntity::create(_game, {"blood" + _to_string(random(1, 5)) + ".png"},
                                                                   random(5, 10), cocos2d::ui::Widget::TextureResType::PLIST);
        blood->setWorldPosition(Vec2(getWorldPosition().x + random(-30, 30),
                                     getWorldPosition().y + random(-30, 30)));
        blood->setScale(0.2f);
        _game->addEntity(blood);
        
        ret = true;
    }
    
    else if ( msg.msg == MessageType::HITTED_BY_AXE )
    {
        AnimatedFiniteEntity* blood = AnimatedFiniteEntity::create(_game, {"big_blood.PNG"},
                                                                   random(5.0f, 10.0f), cocos2d::ui::Widget::TextureResType::PLIST);
        blood->setRotation(random(0.0f, 360.f));
        blood->setWorldPosition(getWorldPosition());
        blood->setScale(0.5f);
        _game->addEntity(blood);
        
        ret = true;
    }
    
    else if ( msg.msg == MessageType::SYNC_INVENTORY_WEAPON_VIEW )
    {
        if ( _uiLayer )
        {
            _uiLayer->syncItemView(_inventoryData);
            _uiLayer->syncWeaponView(_inventoryData);
        }
        
        ret = true;
    }
    
    else if ( msg.msg == MessageType::RELOAD_COMPLETE )
    {
        WeaponBase* equipedWeapon = getEquipedWeapon();
        
        // 인벤토리에있는 총알아이템을 소모한다.
        InventoryData* inventory = getInventoryData();
        EntityType bulletType = equipedWeapon->getBulletType();
        inventory->setItemAmount( bulletType, inventory->getItemAmount(bulletType) - equipedWeapon->getReservedBullets());
        
        // 무기의 탄창을 채운다.
        equipedWeapon->setNumOfLeftRounds(equipedWeapon->getNumOfLeftRounds() +  equipedWeapon->getReservedBullets());
        equipedWeapon->setReservedBullets(0);
        
        // ui 갱신
        _game->sendMessage(0.0, this, nullptr, MessageType::SYNC_INVENTORY_WEAPON_VIEW, nullptr);
        
        ret = true;
    }
    
    else if ( msg.msg == MessageType::MOVE_BALANCE )
    {
        auto offset = static_cast<Vec2*>(msg.extraInfo);
        _balance->setPosition( _balance->getPosition() - *offset );
        CC_SAFE_DELETE(offset);
        
        ret = true;
    }
    
    return ret;
}


void HumanBase::enableNormal(bool enable)
{
    if ( !_animator ) return ;
    
    if ( enable ) _animator->enableNormalMap();
    else _animator->disableNormalMap();
}


void HumanBase::setBrain(BrainBase* brain)
{
    CC_SAFE_DELETE(_brain);
    _brain = brain;
}


void HumanBase::hittedByWeapon(EntityType type, int damage)
{
    if ( type == EntityType::ITEM_M16A2 || type == EntityType::ITEM_M1897 || type == EntityType::ITEM_GLOCK17 )
    {
        ReceiverSenderDamage d;
        d.damage = damage;
        _game->sendMessage(0.0, this, nullptr, MessageType::HITTED_BY_GUN, &d);
    }
    else if ( type == EntityType::ITEM_AXE )
    {
        ReceiverSenderDamage d;
        d.damage = damage;
        _game->sendMessage(0.0, this, nullptr, MessageType::HITTED_BY_AXE, &d);
    }
    
}


void HumanBase::reload()
{
    if ( _equipedWeapon ) _equipedWeapon->reload();
}


int HumanBase::addItem(ItemBase* item)
{
    int slot = _inventoryData->addItem(item);
    // 인벤토리 뷰에 추가함.
    if ( _uiLayer && slot != -1 )
    {
        _uiLayer->syncItemView(_inventoryData);
        _uiLayer->syncWeaponView(_inventoryData);
    }
    return slot;
}


void HumanBase::useItem(EntityType type)
{
    auto item = _inventoryData->getItemType(type);
    if ( item ) item->use();
}


void HumanBase::releaseWeapon(EntityType type)
{
    auto item = _inventoryData->getItemType(type);
    if ( item && isMasked(item->getFamilyMask(), FamilyMask::WEAPON_BASE) )
    {
        if ( _equipedWeapon && _equipedWeapon->getEntityType() == type )
        {
            _equipedWeapon->use();
        }
    }
}


void HumanBase::vibrate()
{
    _game->sendMessage(0.0, this, this, MessageType::MOVE_BALANCE, new Vec2(2.0f * getHeading()));
    _game->sendMessage(0.05 / 3.0, this, this, MessageType::MOVE_BALANCE, new Vec2(2.0f * getHeading()));
    _game->sendMessage(0.10 / 3.0, this, this, MessageType::MOVE_BALANCE, new Vec2(2.0f * getHeading()));
    _game->sendMessage(0.30 / 3.0, this, this, MessageType::MOVE_BALANCE, new Vec2(-2.0f * getHeading()));
    _game->sendMessage(0.35 / 3.0, this, this, MessageType::MOVE_BALANCE, new Vec2(-2.0f * getHeading()));
    _game->sendMessage(0.40 / 3.0, this, this, MessageType::MOVE_BALANCE, new Vec2(-2.0f * getHeading()));
}









