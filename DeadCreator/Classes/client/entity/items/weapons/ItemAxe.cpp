//
//  ItemAxe.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 1. 13..
//
//

#include "ItemAxe.hpp"
#include "HumanBase.hpp"
#include "Game.hpp"
using namespace cocos2d;
using namespace realtrick::client;


ItemAxe::ItemAxe(Game* game) : WeaponBase(game)
{
    setEntityType(ITEM_AXE);
    
    _numOfLeftRounds = 999;
    _maxRounds = 999;
    _damage = 250;
    setRange(64.0f);
    setDelay(2.0f);
    setBulletType(BULLET_AXE);
    _spriteName = "Axe.png";
}


ItemAxe::~ItemAxe()
{}


ItemAxe* ItemAxe::create(Game* game)
{
    ItemAxe* ret = new (std::nothrow)ItemAxe(game);
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



void ItemAxe::discard()
{
}


void ItemAxe::attackImpl()
{
    auto owner = _owner;
    Vec2 worldPos = owner->getWorldPosition();
    
    // 엔티티들과의 충돌처리
    
    ReceiverSenderDamage d2;
    d2.damage = this->getDamage();
    d2.sender = owner;
    
    bool isHit = false;
    Vec2 shootAt = owner->getHeading();
    const auto& members = _game->getNeighborsOnAttack(worldPos, shootAt, this->getRange());
    for (const auto &d : members)
    {
        if ( _game->isAllyState(owner->getPlayerType(), d->getPlayerType()) ) continue;
        
        if ( isMasked(d->getFamilyMask(), FamilyMask::HUMAN_BASE) )
        {
            HumanBase* human = static_cast<HumanBase*>(d);
            if( human->isAlive() && physics::intersect(Segment(worldPos, worldPos + owner->getHeading() * this->getRange()),
                                                       Circle(d->getWorldPosition(), human->getBoundingRadius())) )
            {
                ReceiverSenderDamage s;
                s.damage = this->getDamage();
                s.receiver = human;
                s.sender = owner;
                _game->sendMessage(0.0, human, owner, MessageType::HITTED_BY_AXE, &s);
                
                d2.receiver = human;
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
        _game->pushLogic(0.0, MessageType::PLAY_SOUND, &s);
        
        _game->sendMessage(0.0, owner, owner, MessageType::HIT, &d2);
    }
    else
    {
        SoundSource s;
        s.fileName = "AxeSwing" + _to_string(random(0,2)) + ".mp3";
        s.position = worldPos;
        s.soundRange = 200.0f;
        _game->pushLogic(0.0, MessageType::PLAY_SOUND, &s);
        _game->sendMessage(0.0, owner, owner, MessageType::NO_HIT, nullptr);
    }
}









