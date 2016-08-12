//
//  ItemM1897.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 1. 13..
//
//

#include "ItemM1897.hpp"
#include "HumanBase.hpp"
#include "Game.hpp"
#include "AnimatedFiniteEntity.hpp"
using namespace cocos2d;
using namespace realtrick::client;

ItemM1897::ItemM1897(Game* game) : WeaponBase(game)
{
    setEntityType(ITEM_M1897);
    setRange(200);
    setDamage(20);
    setMaxRounds(5);
    setNumOfLeftRounds(0);
    setDelay(1.5f);
    setNumOfShells(12);
    setBulletType(EntityType::BULLET_SHELL);
}


ItemM1897::~ItemM1897()
{}


ItemM1897* ItemM1897::create(Game* game)
{
    ItemM1897* ret = new (std::nothrow)ItemM1897(game);
    if( ret && ret->init("M1897.png", "M1897.png", "M1897.png", cocos2d::ui::Widget::TextureResType::PLIST))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ItemBase* ItemM1897::clone() const
{
    return new ItemM1897(*this);
}


void ItemM1897::inWeapon()
{
    _owner->getFSM()->changeState(&HumanM1897In::getInstance());
}


void ItemM1897::outWeapon()
{
    _owner->getFSM()->changeState(&HumanM1897Out::getInstance());
}


void ItemM1897::attack()
{
    _game->addLog("<ItemM1897:attack> attack!");
    vector<vector<pair<float, EntityBase*>>> closestIntersectPoint(_numOfShells);
    Vec2 worldPos = _owner->getWorldPosition();
    
    AnimatedFiniteEntity* es = AnimatedFiniteEntity::create(_game, {"ess0.png", "ess1.png", "ess2.png", "ess3.png", "ess4.png" },
                                                            5.0f, ui::Widget::TextureResType::PLIST);
    es->setWorldPosition(worldPos + _owner->getHeading() * random(-30.0f, 30.0f) + _owner->getRight() * random(20.0f, 40.0f));
    es->setScale(0.7f);
    es->setRotation(_owner->getRotationZ());
    _game->addEntity(es);
    
    
    // 엔티티들과의 충돌처리
    Vec2 typicalHeading = _owner->getHeading();
    Segment bulletRay = Segment(worldPos, worldPos + typicalHeading * getRange());
    vector<pair<float, Vec2>> shootAts(_numOfShells);
    for(int i = 0 ; i < _numOfShells ; ++ i)
    {
        Mat3 randomRotation;
        randomRotation.rotate(MATH_DEG_TO_RAD(random(-15.0f, 15.0f)));
        shootAts[i].first = random(getRange() / 3.0f, getRange());
        shootAts[i].second = randomRotation.getTransformedVector(typicalHeading);
    }
    
    const std::list<EntityBase*>& members = _game->getNeighborsOnAttack(worldPos, typicalHeading, getRange());
    for (const auto &d : members)
    {
        if ( d == _owner ) continue;
        
        if ( isMasked(d->getFamilyMask(), FamilyMask::HUMAN_BASE) )
        {
            for(int s = 0 ; s < shootAts.size() ; ++ s)
            {
                HumanBase* human = static_cast<HumanBase*>(d);
                if( human->isAlive() && physics::intersect(Segment(worldPos, worldPos + shootAts[s].second * shootAts[s].first), Circle(d->getWorldPosition(), human->getBoundingRadius())) )
                {
                    closestIntersectPoint[s].push_back(std::make_pair(worldPos.distance(d->getWorldPosition()), d));
                }
            }
        }
    }
    
    // 벽과의 충돌처리
    const std::vector<Polygon> walls = _game->getNeighborWalls(_owner->getWorldPosition(), bulletRay);
    float dist;
    for( const auto& wall : walls )
    {
        for(int s  = 0 ; s < shootAts.size() ; ++ s)
        {
            for( int i = 0 ; i < wall.vertices.size() - 1 ; ++i)
            {
                if ( physics::intersectGet(bulletRay, Segment(wall.vertices[i], wall.vertices[i + 1]), dist) )
                {
                    closestIntersectPoint[s].push_back(std::make_pair(dist, nullptr));
                }
            }
            if ( physics::intersectGet(bulletRay, Segment(wall.vertices.back(), wall.vertices.front()),dist) )
            {
                closestIntersectPoint[s].push_back(std::make_pair(dist, nullptr));
            }
        }
    }
    
    bool isHit = false;
    for(int s = 0 ; s < shootAts.size() ; ++ s)
    {
        if ( closestIntersectPoint[s].empty() == false )
        {
            auto collider = *(min_element(std::begin(closestIntersectPoint[s]), std::end(closestIntersectPoint[s]), [](const std::pair<float, EntityBase*>& p1, const std::pair<float, EntityBase*>& p2) {
                return p1.first < p2.first;
            }));
            
            // 최소거리에 충돌된 충돌체가 사람이면 처리. (벽일수도있음)
            if ( collider.second != nullptr )
            {
                ReceiverSenderDamage s;
                s.receiverID = collider.second->getTag();
                s.senderID = _owner->getTag();
                s.damage = getDamage();
                _game->sendMessage(0.0, collider.second, _owner, MessageType::HITTED_BY_GUN, &s);
                
                isHit = true;
            }
        }
    }
    
    if ( isHit ) _game->sendMessage(0.0, _owner, _owner, MessageType::HIT, nullptr);
    else _game->sendMessage(0.0, _owner, _owner, MessageType::NO_HIT, nullptr);
}


void ItemM1897::discard()
{
}









