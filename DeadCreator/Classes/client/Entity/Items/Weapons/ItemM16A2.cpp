//
//  ItemM16A2.cpp
//  TheDeadForest
//
//  Created by mac on 2016. 1. 13..
//
//

#include "ItemM16A2.hpp"
#include "EntityHuman.hpp"
#include "GameManager.hpp"
#include "EntityEmptyCartridge.hpp"
using namespace cocos2d;
using namespace realtrick::client;

ItemM16A2::ItemM16A2(GameManager* mgr) : WeaponBase(mgr)
{
    setEntityType(ITEM_M16A2);
    setRange(800.0f);
    setDamage(24);
    setMaxRounds(30);
    setNumOfLeftRounds(0);
    setDelay(0.5f);
    setBulletType(EntityType::BULLET_556MM);
}


ItemM16A2::~ItemM16A2()
{}


ItemM16A2* ItemM16A2::create(GameManager* mgr)
{
    ItemM16A2* ret = new (std::nothrow)ItemM16A2(mgr);
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


void ItemM16A2::attack()
{
    if ( _owner->getTag() == _gameMgr->getPlayerPtr()->getTag() )
    {
        // 총쏜사람이 플레이어일 경우 크로스헤어 이벤트를 발동시킨다.
        Dispatch.pushMessage(0.0, _owner, _owner, MessageType::SCREEN_VIBRATE_EVENT, nullptr);
    }
    
    log("<ItemM16A2:attack> attack!");
    std::vector<std::pair<float, EntityBase*>> closestIntersectPoint;
    Vec2 worldPos = _owner->getWorldPosition();
    
    EntityEmptyCartridge* es = EntityEmptyCartridge::create(_gameMgr, {"es0.png", "es1.png", "es2.png", "es3.png", "es4.png" }, 5.0f, ui::Widget::TextureResType::PLIST);
    es->setWorldPosition(worldPos + _owner->getHeading() * random(-30.0f, 30.0f) + _owner->getRight() * random(20.0f, 40.0f));
    es->setRotation(_owner->getBodyRot());
    _gameMgr->addDynamicEntity(es, Z_ORDER_ITEMS, _gameMgr->getNextValidID());
    
    // 엔티티들과의 충돌처리
    Mat3 rotMat;
    rotMat.rotate(MATH_DEG_TO_RAD(random(-3.0f, 3.0f)));
    Vec2 shootAt = rotMat.getTransformedVector(_owner->getHeading());
    Segment bulletRay = Segment(worldPos, worldPos + shootAt * getRange());
    
    const std::list<EntityBase*>& members = _gameMgr->getNeighborsOnAttack(worldPos, shootAt, getRange());
    for (const auto &d : members)
    {
        if ( d == _owner ) continue;
        
        if ( d->getEntityType() == ENTITY_HUMAN)
        {
            EntityHuman* human = static_cast<EntityHuman*>(d);
            if( human->isAlive() && physics::intersect(Segment(worldPos, worldPos + _owner->getHeading() * getRange()), Circle(d->getWorldPosition(), 20.0f)) )
            {
                closestIntersectPoint.push_back(std::make_pair(worldPos.distance(d->getWorldPosition()), d));
            }
            
            // DEBUG:
            _gameMgr->getDebugNode()->drawCircle(_gameMgr->worldToLocal(worldPos), 20.0f, 360.0f, 20, false, Color4F::YELLOW);
        }
    }
    
    // 벽과의 충돌처리
    const std::vector<Polygon> walls = _gameMgr->getNeighborWalls(_owner->getWorldPosition(), bulletRay);
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
            log("<WeaponBase::attack> hit at (%.0f, %.0f), id: %d.", collider.second->getWorldPosition().x, collider.second->getWorldPosition().y, collider.second->getTag());
            
            ReceiverSenderDamage d;
            d.receiverID = collider.second->getTag();
            d.senderID = _owner->getTag();
            d.damage = getDamage();
            Dispatch.pushMessage(0.0, collider.second, _owner, MessageType::HITTED_BY_GUN, &d);
            
            SoundSource s;
            s.fileName = "GunShotAt" + GameManager::_to_string(random(0, 1)) + ".mp3";
            s.position = worldPos + shootAt * collider.first;
            s.soundRange = 2000.0f;
            Dispatch.pushMessage(0.0, _owner, _owner, MessageType::PLAY_SOUND, &s);
            
            if ( _owner->getTag() == _gameMgr->getPlayerPtr()->getTag() )
            {
                // 총쏜사람이 플레이어일 경우 크로스헤어 이벤트를 발동시킨다.
                Dispatch.pushMessage(0.0, _owner, _owner, MessageType::CROSS_HAIR_EVENT, nullptr);
            }
        }
        
        // DEBUG:
        if ( _gameMgr->getDebugNode()->isVisible() )
        {
            _gameMgr->getDebugNode()->drawSegment(_gameMgr->worldToLocal(worldPos), _gameMgr->worldToLocal(worldPos + shootAt * collider.first), 0.5f, Color4F::YELLOW);
            _gameMgr->getDebugNode()->drawDot(_gameMgr->worldToLocal(worldPos + shootAt * collider.first), 5.0f, Color4F::RED);
        }
    }
    else
    {
        // DEBUG:
        if ( _gameMgr->getDebugNode()->isVisible() )
        {
            _gameMgr->getDebugNode()->drawSegment(_gameMgr->worldToLocal(worldPos), _gameMgr->worldToLocal(worldPos + shootAt * getRange()), 0.5f, Color4F::YELLOW);
        }
    }
}


void ItemM16A2::discard()
{
    ItemM16A2* item = ItemM16A2::create(_gameMgr);
    item->setAmount( getAmount() );
    item->setNumOfLeftRounds(getNumOfLeftRounds());
    item->setPosition(Vec2(_owner->getPosition().x + 50.0f, _owner->getPosition().y));
    _gameMgr->addDynamicEntity(item, Z_ORDER_ITEMS, _gameMgr->getNextValidID());
}









