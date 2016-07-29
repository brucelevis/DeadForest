//
//  DynamicEntity.cpp
//  TheDeadForest
//
//  Created by mac on 2016. 1. 2..
//
//

#include "DynamicEntity.hpp"
#include "GameManager.hpp"

using namespace cocos2d;
using namespace realtrick::client;

DynamicEntity::DynamicEntity(GameManager* gameMgr) :
EntityBase(gameMgr),
_heading(Vec2::UNIT_X),
_targetHeading(Vec2::UNIT_X),
_moving(Vec2::UNIT_X),
_left(Vec2::ZERO),
_right(Vec2::ZERO),
_velocity(Vec2::ZERO),
_force(Vec2::ZERO),
_acceleration(Vec2::ZERO),
_mass(1.0f),
_maxSpeed(0.0f),
_turnSpeed(0.0f),
_speed(0.0f)
{
    ADD_FAMILY_MASK(_familyMask, DYNAMIC_ENTITY);
}

DynamicEntity::~DynamicEntity()
{
    
}

void DynamicEntity::moveEntity()
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
    const std::list<EntityBase*> members = _gameMgr->getNeighborsOnMove(oldPos, _speed);
    for ( const auto &entity : members )
    {
        if ( entity == this ) continue;
        
        if ( isIntersectOther(futurePosition, entity) )
        {
            intersectResult = true;
        }
    }
    
    // 벽과의 충돌처리
    const std::vector<Polygon> walls = _gameMgr->getNeighborWalls(futurePosition, _speed);
    for( const auto& wall : walls )
    {
        if ( isIntersectWall(futurePosition, wall) )
        {
            intersectResult = true;
        }
    }
    
    if ( !intersectResult )
    {
        setWorldPosition(futurePosition);
        _gameMgr->getCellSpace()->updateEntity(this, oldPos);
    }
}









