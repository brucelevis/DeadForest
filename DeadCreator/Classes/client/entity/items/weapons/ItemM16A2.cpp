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









