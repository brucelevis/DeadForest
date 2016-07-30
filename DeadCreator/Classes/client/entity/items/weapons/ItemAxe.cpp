//
//  ItemAxe.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 1. 13..
//
//

#include "ItemAxe.hpp"
#include "EntityHuman.hpp"
#include "GameManager.hpp"
#include "GameWorld.hpp"
using namespace cocos2d;
using namespace realtrick::client;


ItemAxe::ItemAxe(GameManager* mgr) : WeaponBase(mgr)
{
    setEntityType(ITEM_AXE);
    
    _numOfLeftRounds = 999;
    _maxRounds = 999;
    _damage = 100;
    setRange(64.0f);
    setDelay(2.0f);
    setBulletType(BULLET_AXE);
}


ItemAxe::~ItemAxe()
{}


ItemAxe* ItemAxe::create(GameManager* mgr)
{
    ItemAxe* ret = new (std::nothrow)ItemAxe(mgr);
    if( ret && ret->init("Axe.png", "Axe.png", "Axe.png", cocos2d::ui::Widget::TextureResType::PLIST))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


ItemBase* ItemAxe::clone() const
{
    return new ItemAxe(*this);
}


void ItemAxe::inWeapon()
{
    _owner->getFSM()->changeState(&HumanAxeIn::getInstance());
}


void ItemAxe::outWeapon()
{
    _owner->getFSM()->changeState(&HumanAxeOut::getInstance());
}


void ItemAxe::attack()
{
    log("<ItemAxe:attack> attack!");
    Vec2 worldPos = _owner->getWorldPosition();
    
    // 엔티티들과의 충돌처리
    bool isHit = false;
    Vec2 shootAt = _owner->getHeading();
    const std::list<EntityBase*>& members = _gameMgr->getNeighborsOnAttack(worldPos, shootAt, getRange());
    for (const auto &d : members)
    {
        if ( d == _owner ) continue;
        
        if ( d->getEntityType() == ENTITY_HUMAN)
        {
            EntityHuman* human = static_cast<EntityHuman*>(d);
            if( human->isAlive() && physics::intersect(Segment(worldPos, worldPos + _owner->getHeading() * getRange()), Circle(d->getWorldPosition(), human->getBoundingRadius())) )
            {
                log("<WeaponBase::attack> hit at (%.0f, %.0f), id: %d.", human->getWorldPosition().x, human->getWorldPosition().y, human->getTag());
                
                ReceiverSenderDamage s;
                s.damage = getDamage();
                s.receiverID = d->getTag();
                s.senderID = _owner->getTag();
                Dispatch.pushMessage(0.0, human, _owner, MessageType::HITTED_BY_AXE, &s);
                
                isHit = true;
            }
        }
    }
    
    if ( isHit )
    {
        SoundSource s;
        s.fileName = "AxeHit" + _to_string(random(0,2)) + ".mp3";
        s.position = worldPos;
        s.soundRange = 200.0f;
        Dispatch.pushMessage(0.0, _owner, _owner, MessageType::PLAY_SOUND, &s);
        
        if ( _owner->getTag() == _gameMgr->getPlayerPtr()->getTag() )
        {
            // 총쏜사람이 플레이어일 경우 크로스헤어 이벤트를 발동시킨다.
            Dispatch.pushMessage(0.0, _owner, _owner, MessageType::CROSS_HAIR_EVENT, nullptr);
        }
    }
    else
    {
        SoundSource s;
        s.fileName = "AxeSwing" + _to_string(random(0,2)) + ".mp3";
        s.position = worldPos;
        s.soundRange = 200.0f;
        Dispatch.pushMessage(0.0, _owner, _owner, MessageType::PLAY_SOUND, &s);
    }
}


void ItemAxe::discard()
{
    ItemAxe* item = ItemAxe::create(_gameMgr);
    item->setAmount( getAmount() );
    item->setNumOfLeftRounds(getNumOfLeftRounds());
    item->setPosition(Vec2(_owner->getPosition().x + 50.0f, _owner->getPosition().y));
    _gameMgr->addEntity(item, Z_ORDER_ITEMS, _gameMgr->getNextValidID());
}









