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

#include "SimpleProfiler.hpp"

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
_dizzyScale(0.0f),
_bloodyScale(0.0f),
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
_stateName("idle"),
_regulator(0.1f),
_healRegulator(1.0f)
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
    
    CC_SAFE_DELETE(_pathPlanner);
    CC_SAFE_DELETE(_sensory);
    CC_SAFE_DELETE(_targetSystem);
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
    _targetSystem = new AbstTargetingSystem(this);
    
    _nameTag = ui::Text::create("","fonts/SpecialElite.TTF", 10);
    _nameTag->setPosition(Vec2(0.0f, -30.0f));
    addChild(_nameTag);
    
    // create box2d's physics body
    b2BodyDef bd;
    bd.type = b2BodyType::b2_dynamicBody;
    bd.allowSleep = false;
    
    _body = _game->getPhysicsWorld()->CreateBody(&bd);
    
    b2CircleShape shape;
    shape.m_p = b2Vec2(0.0f, 0.0f);
    shape.m_radius = _boundingRadius;
    
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 20.0f;
    fd.friction = 1.0f;
    fd.restitution = 0.0f;
    fd.isSensor = false;
    _body->CreateFixture(&fd);
    
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
    if (_brain && isAlive() && _regulator.isReady())
    {
        _brain->think();
        _sensory->updateVision();
        _targetSystem->update();
    }
    
    if ( _FSM ) _FSM->update(dt);
    
    // move and rotate
    this->moveEntity();
    
    this->rotateEntity();
    
    // self heal
    if ( isAlive() && _healRegulator.isReady() && _blood < _maxBlood )
    {
        _blood += 1;
    }
    
    // only apply to player
    if ( _uiLayer )
    {
        float h = _blood / static_cast<float>(_maxBlood);
        h = cocos2d::clampf(h, 0.0f, 1.0f);
        
        _uiLayer->setHitPoint(h);
        _dizzyScale = 1.0f - h;
        
        if ( _bloodyScale > 0.0f )
        {
            _bloodyScale -= dt;
        }
        else
        {
            _bloodyScale = 0.0f;
        }
    }
    
    // this->setFootGauge( _footGauge + _speed * dt );
    
    // update animation
    if ( _animator )
    {
        _animator->setRotation(_rotation);
        _animator->processAnimation(dt);
    }
}


bool HumanBase::isIntersectOther(const cocos2d::Vec2& futurePosition, EntityBase* other, cocos2d::Vec2& additionalVelocity)
{
    if ( isMasked(other->getFamilyMask(), HUMAN_BASE) )
    {
        HumanBase* human = static_cast<HumanBase*>(other);
        
        if (!human->isAlive()) return false;
        
        cocos2d::Vec2 entityPos = human->getWorldPosition();
        float overlap = std::max(0.0f, human->getBoundingRadius() + _boundingRadius - entityPos.distance(futurePosition));
        
        additionalVelocity += (futurePosition - entityPos).getNormalized() * (overlap / 2);
        
        return (overlap > 0.0f ? true : false);
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
//    if (getVelocity() == Vec2::ZERO)
//    {
//        _speed = 0.0f;
//        return;
//    }
//    
//    float dt = Director::getInstance()->getDeltaTime();
//    cocos2d::Vec2 oldPos = getWorldPosition();
//    cocos2d::Vec2 futurePosition = getWorldPosition() + getVelocity() * dt;
//    _speed = getVelocity().getLength();
//    bool intersectResult = false;
//    cocos2d::Vec2 move;
//    
//    // 엔티티들과의 충돌처리
//    const auto& members = _game->getNeighborsOnMove(oldPos, _speed);
//    for (const auto &entity : members)
//    {
//        if (entity == this) continue;
//        
//        if (isIntersectOther(futurePosition, entity, move))
//            intersectResult = true;
//    }
//    
//    // 벽과의 충돌처리
//    futurePosition += move;
//    float overlap = 0.0f;
//    int collideCnt = 0;
//    Vec2 beginSum, endSum;
//    const std::vector<realtrick::Polygon>& walls = _game->getNeighborSimpleWalls(futurePosition, _speed);
//    for (const auto& wall : walls)
//    {
//        for (int i = 0; i < wall.vertices.size() - 1; ++i)
//        {
//            Vec2 begin(wall.vertices[i]);
//            Vec2 end(wall.vertices[i + 1]);
//            
//            float distance = physics::distToSegment(begin, end, futurePosition);
//            if (distance < _boundingRadius)
//            {
//                collideCnt++;
//                overlap += distance;
//                beginSum += begin;
//                endSum += end;
//            }
//        }
//        
//        Vec2 begin(wall.vertices.back());
//        Vec2 end(wall.vertices.front());
//        
//        float distance = physics::distToSegment(begin, end, futurePosition);
//        if (distance < _boundingRadius)
//        {
//            collideCnt++;
//            overlap += distance;
//            beginSum += begin;
//            endSum += end;
//        }
//    }
//    
//    if (collideCnt > 0)
//    {
//        overlap /= collideCnt;
//        move += (beginSum - endSum).getPerp().getNormalized() * (overlap * 0.33f);
//    }
//    
//    setWorldPosition(futurePosition + move);
//    _game->getCellSpace()->updateEntity(this, oldPos);
}

void HumanBase::rotateEntity()
{
//    if ( _heading.dot(_targetHeading) < 0.982546f )
//    {
//        float dt = Director::getInstance()->getDeltaTime();
//        
//        float d = getHeading().cross(_targetHeading);
//        if( d > 0 )
//        {
//            Mat3 rotMat;
//            rotMat.rotate(MATH_DEG_TO_RAD(_turnSpeed * dt));
//            setHeading(rotMat.getTransformedVector(getHeading()));
//        }
//        else
//        {
//            Mat3 rotMat;
//            rotMat.rotate(-MATH_DEG_TO_RAD(_turnSpeed * dt));
//            setHeading(rotMat.getTransformedVector(getHeading()));
//        }
//        
//        setRotationZ(-physics::getAngleFromZero(getHeading()));
//    }
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
        
        _game->pushLogic(0.0, MessageType::PLAY_SOUND, &s);
        
        return ;
    }
    
    _footGauge = g;
}


bool HumanBase::handleMessage(const Telegram& msg)
{
    bool ret = false;
    
    if ( _FSM ) ret = _FSM->handleMessage(msg);
    
    if ( msg.msg  == MessageType::HITTED_BY_GUN )
    {
        AnimatedFiniteEntity* blood = AnimatedFiniteEntity::create(_game, {"bloody" + _to_string(random(1, 6)) + ".png"},
                                                                   random(5, 10), cocos2d::ui::Widget::TextureResType::PLIST);
        blood->setRotation(random(0.0f, 360.f));
        blood->setWorldPosition(Vec2(getWorldPosition().x + random(-30, 30),
                                     getWorldPosition().y + random(-30, 30)));
        _game->addEntity(blood);
        
        
        AnimatedFiniteEntity* bloody = AnimatedFiniteEntity::create(_game, {
            "bloody1.png",
            "bloody2.png",
            "bloody3.png",
            "bloody4.png",
            "bloody5.png",
            "bloody6.png",
            "bloody7.png"
        }, 0.0f, cocos2d::ui::Widget::TextureResType::PLIST);
        bloody->setWorldPosition(Vec2(getWorldPosition().x + random(-30, 30),
                                     getWorldPosition().y + random(-30, 30)));
        _game->addEntity(bloody, 10);
        
        ret = true;
    }
    
    else if ( msg.msg == MessageType::HITTED_BY_AXE )
    {
        AnimatedFiniteEntity* blood = AnimatedFiniteEntity::create(_game, {"bloody" + _to_string(random(2, 4)) + ".png"},
                                                                   random(5.0f, 10.0f), cocos2d::ui::Widget::TextureResType::PLIST);
        blood->setRotation(random(0.0f, 360.f));
        blood->setWorldPosition(Vec2(getWorldPosition().x + random(-30, 30),
                                     getWorldPosition().y + random(-30, 30)));
        _game->addEntity(blood);
        
        AnimatedFiniteEntity* bloody = AnimatedFiniteEntity::create(_game, {
            "bloody1.png",
            "bloody2.png",
            "bloody3.png",
            "bloody4.png",
            "bloody5.png",
            "bloody6.png",
            "bloody7.png"
        }, 0.0f, cocos2d::ui::Widget::TextureResType::PLIST);
        bloody->setWorldPosition(Vec2(getWorldPosition().x + random(-30, 30),
                                      getWorldPosition().y + random(-30, 30)));
        _game->addEntity(bloody, 10);

        
        ret = true;
    }
    
    else if ( msg.msg == MessageType::HITTED_BY_FIST )
    {
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
    
    else if ( msg.msg == MessageType::HIT )
    {
        // only apply to player
        if ( _uiLayer )
        {
            _uiLayer->runCrossHairEffect("hit");
            
            ReceiverSenderDamage* d = static_cast<ReceiverSenderDamage*>(msg.extraInfo);
            _game->addLog("zombie's blood is " + _to_string(d->receiver->getBlood()) + ".");
        }
        
        ret = true;
    }
    
    else if ( msg.msg == MessageType::NO_HIT )
    {
        // only apply to player
        if ( _uiLayer ) _uiLayer->runCrossHairEffect("fire");
        
        ret = true;
    }
    
    // calc damage
    if ( msg.msg == MessageType::HITTED_BY_GUN ||
        msg.msg == MessageType::HITTED_BY_AXE ||
        msg.msg == MessageType::HITTED_BY_FIST )
    {
        ReceiverSenderDamage* d = static_cast<ReceiverSenderDamage*>(msg.extraInfo);
        if ( _blood > 0 )
        {
            _blood -= d->damage;
            _blood = clampf(_blood, 0.0f, _maxBlood);
            
            if ( d->receiver->getTag() == _game->getPlayerPtr()->getTag() )
            {
                _bloodyScale = 1.0f;
                
                SoundSource s;
                s.fileName = "PlayerScreamSound_" + _to_string(random(1, 3)) + ".mp3";
                s.position = static_cast<HumanBase*>(d->sender)->getWorldPosition();
                s.soundRange = 1000.0f;
                s.volume = 1.0f;
                _game->pushLogic(0.0, MessageType::PLAY_SOUND, &s);
            }
        }
        
        if ( _blood <= 0 && isAlive() )
        {
            if ( d->sender->getTag() == _game->getPlayerPtr()->getTag() )
            {
                SoundSource s;
                s.fileName = "kill_ui_sound.mp3";
                s.position = static_cast<HumanBase*>(d->sender)->getWorldPosition();
                s.soundRange = 1000.0f;
                s.volume = 1.0f;
                _game->pushLogic(0.0, MessageType::PLAY_SOUND, &s);
                
            }
            
            this->suicide();
        }
        
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


void HumanBase::reload()
{
    if ( _equipedWeapon ) _equipedWeapon->reload();
}


void HumanBase::attackByWeapon()
{
    if ( _equipedWeapon ) _equipedWeapon->attack();
}


void HumanBase::attackByFist()
{
    if ( !_equipedWeapon )
    {
        Vec2 worldPos = this->getWorldPosition();
        
        // 엔티티들과의 충돌처리
        bool isHit = false;
        Vec2 shootAt = this->getHeading();
        const auto& members = _game->getNeighborsOnAttack(worldPos, shootAt, 40.0f);
        for (const auto &d : members)
        {
            if ( _game->isAllyState(getPlayerType(), d->getPlayerType()) ) continue;
            
            if ( isMasked(d->getFamilyMask(), FamilyMask::HUMAN_BASE) )
            {
                HumanBase* human = static_cast<HumanBase*>(d);
                if( human->isAlive() && physics::intersect(Segment(worldPos, worldPos + this->getHeading() * 40.0f),
                                                           Circle(d->getWorldPosition(), human->getBoundingRadius())) )
                {
                    ReceiverSenderDamage s;
                    s.damage = 10.0f;
                    s.receiver = human;
                    s.sender = this;
                    _game->sendMessage(0.0, human, this, MessageType::HITTED_BY_FIST, &s);
                    
                    isHit = true;
                }
            }
        }
        
        if ( isHit )
        {
            _game->sendMessage(0.0, this, this, MessageType::HIT, nullptr);
        }
        else
        {
            _game->sendMessage(0.0, this, this, MessageType::NO_HIT, nullptr);
        }
    }
}









