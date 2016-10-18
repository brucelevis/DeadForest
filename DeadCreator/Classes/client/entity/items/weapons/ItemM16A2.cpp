//
//  ItemM16A2.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 1. 13..
//
//

#include "ItemM16A2.hpp"
#include "HumanBase.hpp"
#include "Game.hpp"
#include "AnimatedFiniteEntity.hpp"
using namespace cocos2d;
using namespace realtrick::client;

ItemM16A2::ItemM16A2(Game* game) : WeaponBase(game)
{
    setEntityType(ITEM_M16A2);
    setRange(800.0f);
    setDamage(24);
    setMaxRounds(30);
    setNumOfLeftRounds(0);
    setDelay(0.5f);
    setBulletType(EntityType::BULLET_556MM);
    _spriteName = "M16A2.png";
}


ItemM16A2::~ItemM16A2()
{}


ItemM16A2* ItemM16A2::create(Game* game)
{
    ItemM16A2* ret = new (std::nothrow)ItemM16A2(game);
    if( ret && ret->init("M16A2.png", "M16A2.png", "M16A2.png", cocos2d::ui::Widget::TextureResType::PLIST))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


ItemBase* ItemM16A2::clone() const
{
    return new ItemM16A2(*this);
}


void ItemM16A2::inWeapon()
{
    _owner->getFSM()->changeState(&HumanM16A2In::getInstance());
}


void ItemM16A2::outWeapon()
{
    _owner->getFSM()->changeState(&HumanM16A2Out::getInstance());
}


void ItemM16A2::discard()
{
}


void ItemM16A2::dropCartiridges()
{
    Vec2 worldPos = _owner->getWorldPosition();
    
    AnimatedFiniteEntity* es = AnimatedFiniteEntity::create(_game, {"es0.png", "es1.png", "es2.png", "es3.png", "es4.png" }, 5.0f, ui::Widget::TextureResType::PLIST);
    es->setWorldPosition(worldPos + _owner->getHeading() * random(-30.0f, 30.0f) + _owner->getRight() * random(20.0f, 40.0f));
    es->setRotation(_owner->getRotationZ());
    _game->addEntity(es);
}


void ItemM16A2::attackImpl()
{
    this->setNumOfLeftRounds( getNumOfLeftRounds() - 1 );
    
    auto owner = _owner;
    owner->attackVibrate(0.5f);
    
    std::vector<std::pair<float, EntityBase*>> closestIntersectPoint;
    Vec2 worldPos = owner->getWorldPosition();
    
    // 엔티티들과의 충돌처리
    Mat3 rotMat;
    rotMat.rotate(MATH_DEG_TO_RAD(random(-3.0f, 3.0f)));
    Vec2 shootAt = rotMat.getTransformedVector(owner->getHeading());
    Segment bulletRay = Segment(worldPos, worldPos + shootAt * this->getRange());
    
    const auto& members = _game->getNeighborsOnAttack(worldPos, shootAt, this->getRange());
    for (const auto &d : members)
    {
        if ( _game->isAllyState(owner->getPlayerType(), d->getPlayerType()) ) continue;
        
        if ( isMasked(d->getFamilyMask(), FamilyMask::HUMAN_BASE) )
        {
            HumanBase* human = static_cast<HumanBase*>(d);
            if( human->isAlive() && physics::intersect(Segment(worldPos,
                                                               worldPos + owner->getHeading() * this->getRange()), Circle(d->getWorldPosition(), 20.0f)) )
            {
                closestIntersectPoint.push_back(std::make_pair(worldPos.distance(d->getWorldPosition()), d));
            }
        }
    }
    
    // 벽과의 충돌처리
    const std::vector<Polygon> walls = _game->getNeighborWalls(owner->getWorldPosition(), bulletRay);
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
        
        // 최소거리에 충돌된 충돌체가 사람이면 처리. (벽일수도있음)
        if ( collider.second != nullptr )
        {
            ReceiverSenderDamage d;
            d.receiver = static_cast<HumanBase*>(collider.second);
            d.sender = owner;
            d.damage = this->getDamage();
            _game->sendMessage(0.0, collider.second, owner, MessageType::HITTED_BY_GUN, &d);
            
            SoundSource s;
            s.fileName = "GunShotAt" + _to_string(random(0, 1)) + ".mp3";
            s.position = worldPos;
            s.soundRange = 2000.0f;
            _game->pushLogic(0.0, MessageType::PLAY_SOUND, &s);
            
            _game->sendMessage(0.0, owner, owner, MessageType::HIT, nullptr);
        }
        else
        {
            _game->sendMessage(0.0, owner, owner, MessageType::NO_HIT, nullptr);
        }
    }
    else
    {
        _game->sendMessage(0.0, owner, owner, MessageType::NO_HIT, nullptr);
    }
}










