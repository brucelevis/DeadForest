//
//  AimingSystem.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 5. 16..
//
//

#include "AimingSystem.hpp"
#include "Game.hpp"
#include "WeaponBase.hpp"

using namespace realtrick::client;
using namespace cocos2d;

AimingSystem::AimingSystem(Game* game, EntityHuman* owner) :
_game(game),
_owner(owner),
_isEnableSystem(false),
_isHitableEntity(false),
_closestHitableEntity(nullptr)
{}


void AimingSystem::update()
{
    if ( !_owner ) return ;
    
    WeaponBase* equipedWeapon = _owner->getEquipedWeapon();
    
    if ( equipedWeapon == nullptr )
    {
        // 주먹을 때.
    }
    else
    {
        Vec2 worldPos = _owner->getWorldPosition();
        Vec2 shootAt = _owner->getHeading();
        std::vector<std::pair<float, EntityBase*>> closestIntersectPoint;
        Segment bulletRay = Segment(worldPos, worldPos + shootAt * equipedWeapon->getRange());

        const std::list<EntityBase*>& members = _game->getNeighborsOnAttack(worldPos, shootAt, equipedWeapon->getRange());
        for (const auto &d : members)
        {
            if ( d == _owner ) continue;
            
            if ( d->getEntityType() == ENTITY_HUMAN)
            {
                EntityHuman* human = (EntityHuman*)d;
                if( human->isAlive() && physics::intersect(bulletRay, Circle(d->getWorldPosition(), 20.0f)) )
                {
                    closestIntersectPoint.push_back(std::make_pair(worldPos.distance(d->getWorldPosition()), d));
                }
            }
        }
        
        // 벽과의 충돌처리
        const std::vector<Polygon> walls = _game->getNeighborWalls(_owner->getWorldPosition(), bulletRay);
        float dist;
        for( const auto& wall : walls )
        {
            for( int i = 0 ; i < wall.vertices.size() - 1 ; ++i)
            {
                if ( physics::intersectGet(bulletRay, Segment(wall.vertices[i], wall.vertices[i + 1]), dist) )
                {
                    closestIntersectPoint.push_back(std::make_pair(dist, nullptr));
                }
            }
            if ( physics::intersectGet(bulletRay, Segment(wall.vertices.back(), wall.vertices.front()),dist) )
            {
                closestIntersectPoint.push_back(std::make_pair(dist, nullptr));
            }
        }
        
        if ( closestIntersectPoint.empty() == false )
        {
            auto collider = *(min_element(std::begin(closestIntersectPoint), std::end(closestIntersectPoint), [](const std::pair<float, EntityBase*>& p1, const std::pair<float, EntityBase*>& p2) {
                return p1.first < p2.first;
            }));
            
            if ( collider.second != nullptr )
            {
                // 최소거리에 충돌된 충돌체가 사람이면 처리.
                _closestHitableEntity = collider.second;
                _isHitableEntity = true;
            }
            else
            {
                // 충돌체가 벽일 떄
                _isHitableEntity = false;
                _closestHitableEntity = nullptr;
            }
        }
        else
        {
            // 충돌체가 아예 없을 때
            _isHitableEntity = false;
            _closestHitableEntity = nullptr;
        }
    }
}









