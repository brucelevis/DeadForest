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
#include "RenderingSystem.hpp"
#include "ZombieBrain.hpp"

using namespace cocos2d;
using namespace realtrick::client;

HumanBase::HumanBase(Game* game) : EntityBase(game),
_heading(Vec2::UNIT_X),
_targetHeading(Vec2::UNIT_X),
_moving(Vec2::UNIT_X),
_left(Vec2::ZERO),
_right(Vec2::ZERO),
_velocity(Vec2::ZERO),
_maxSpeed(0.0f),
_turnSpeed(0.0f),
_speed(0.0f),
_animator(nullptr),
_brain(nullptr),
_inputMask(0),
_blood(0),
_maxBlood(0),
_isAlive(false),
_isRun(false),
_isFovOn(false),
_walkSpeed(Prm.getValueAsFloat("walkSpeed")),
_runSpeed(Prm.getValueAsFloat("runSpeed")),
_footGauge(0.0f),
_rotation(0.0f),
_stateName("idle")
{
    ADD_FAMILY_MASK(_familyMask, HUMAN_BASE);
    setBoundingRadius(Prm.getValueAsFloat("boundingRadius"));
    setTurnSpeed(Prm.getValueAsFloat("turnSpeed"));
}


HumanBase::~HumanBase()
{
    CC_SAFE_DELETE(_animator);
}


bool HumanBase::init()
{
    if ( !Node::init() )
        return false;
    
    _animator = new Animator(this);
    
    _maxBlood = Prm.getValueAsInt("maxBlood");
    _blood = _maxBlood;
    
    setAlive();
    
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
    if ( _brain ) _brain->think();
    if ( _FSM ) _FSM->update(dt);
    
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


bool HumanBase::isIntersectWall(const cocos2d::Vec2& futurePosition, const Polygon& wall)
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
    const std::vector<Polygon> walls = _game->getNeighborWalls(futurePosition, _speed);
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
    
    setRotationZ(-physics::getAngleFromZero(getHeading()));
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
        
        if ( onTile == TileType::DIRT ) s.fileName = "Dirt" + _to_string(random(1, 4)) + ".mp3";
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
        float t = (1.0f - (s->position - _game->getRenderingSysetm()->getCameraPosition()).getLength() / s->soundRange) * s->volume;
        experimental::AudioEngine::setVolume( experimental::AudioEngine::play2d(s->fileName), t);
        
        return true;
    }
    
    return false;
}









