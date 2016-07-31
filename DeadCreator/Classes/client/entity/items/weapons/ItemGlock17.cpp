//
//  ItemGlock17.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 1. 13..
//
//

#include "ItemGlock17.hpp"
#include "EntityHuman.hpp"
#include "Game.hpp"
#include "AnimatedFiniteEntity.hpp"
using namespace cocos2d;
using namespace realtrick::client;

ItemGlock17::ItemGlock17(Game* game) : WeaponBase(game)
{
    setEntityType(ITEM_GLOCK17);
    setRange(800.0f);
    setDamage(51);
    setMaxRounds(17);
    setNumOfLeftRounds(0);
    setDelay(0.5f);
    setBulletType(EntityType::BULLET_9MM);
}


ItemGlock17::~ItemGlock17()
{}


ItemGlock17* ItemGlock17::create(Game* game)
{
    ItemGlock17* ret = new (std::nothrow)ItemGlock17(game);
    if( ret && ret->init("Glock17.png", "Glock17.png", "Glock17.png", cocos2d::ui::Widget::TextureResType::PLIST))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


ItemBase* ItemGlock17::clone() const
{
    return new ItemGlock17(*this);
}


ItemGlock17::ItemGlock17(const ItemGlock17& rhs) : WeaponBase(rhs)
{}


void ItemGlock17::inWeapon()
{
    _owner->getFSM()->changeState(&HumanGlock17In::getInstance());
}


void ItemGlock17::outWeapon()
{
    _owner->getFSM()->changeState(&HumanGlock17Out::getInstance());
}


void ItemGlock17::attack()
{
    log("<ItemGlock17:attack> attack!");
    std::vector<std::pair<float, EntityBase*>> closestIntersectPoint;
    Vec2 worldPos = _owner->getWorldPosition();
    
    AnimatedFiniteEntity* es = AnimatedFiniteEntity::create(_game, {"es0.png", "es1.png", "es2.png", "es3.png", "es4.png" }, 5.0f, ui::Widget::TextureResType::PLIST);
    es->setWorldPosition(worldPos + _owner->getHeading() * random(-30.0f, 30.0f) + _owner->getRight() * random(20.0f, 40.0f));
    es->setRotation(_owner->getBodyRot());
    _game->addEntity(es, Z_ORDER_ITEMS, _game->getNextValidID());
    
    // 엔티티들과의 충돌처리
    Mat3 rotMat;
    rotMat.rotate(MATH_DEG_TO_RAD(random(-3.0f, 3.0f)));
    Vec2 shootAt = rotMat.getTransformedVector(_owner->getHeading());
    Segment bulletRay = Segment(worldPos, worldPos + shootAt * getRange());
    
    const std::list<EntityBase*>& members = _game->getNeighborsOnAttack(worldPos, shootAt, getRange());
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
            s.fileName = "GunShotAt" + _to_string(random(0, 1)) + ".mp3";
            s.position = worldPos + shootAt * collider.first;
            s.soundRange = 2000.0f;
            Dispatch.pushMessage(0.0, _owner, _owner, MessageType::PLAY_SOUND, &s);
            
            if ( _owner->getTag() == _game->getPlayerPtr()->getTag() )
            {
                // 총쏜사람이 플레이어일 경우 크로스헤어 이벤트를 발동시킨다.
                Dispatch.pushMessage(0.0, _owner, _owner, MessageType::CROSS_HAIR_EVENT, nullptr);
            }
        }
        
    }
}


void ItemGlock17::discard()
{
    ItemGlock17* item = ItemGlock17::create(_game);
    item->setAmount( getAmount() );
    item->setNumOfLeftRounds( getNumOfLeftRounds() );
    item->setPosition(Vec2(_owner->getPosition().x + 50.0f, _owner->getPosition().y));
    _game->addEntity(item, Z_ORDER_ITEMS, _game->getNextValidID());
}









