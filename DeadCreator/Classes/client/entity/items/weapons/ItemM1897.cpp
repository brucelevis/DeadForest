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
    setDelay(1.1f);
    setNumOfShells(12);
    setBulletType(EntityType::BULLET_SHELL);
    _spriteName = "M1897.png";
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


void ItemM1897::discard()
{
}


void ItemM1897::dropCartiridges()
{
    Vec2 worldPos = _owner->getWorldPosition();
    
    AnimatedFiniteEntity* es = AnimatedFiniteEntity::create(_game, {"ess0.png",
        "ess1.png",
        "ess2.png",
        "ess3.png",
        "ess4.png" },
                                                            5.0f, ui::Widget::TextureResType::PLIST);
    es->setWorldPosition(worldPos + _owner->getHeading() * random(-30.0f, 30.0f) + _owner->getRight() * random(20.0f, 40.0f));
    es->setScale(0.7f);
    es->setRotation(_owner->getRotationZ());
    _game->addEntity(es);
}







