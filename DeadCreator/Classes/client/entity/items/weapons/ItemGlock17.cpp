//
//  ItemGlock17.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 1. 13..
//
//

#include "ItemGlock17.hpp"
#include "HumanBase.hpp"
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
    _spriteName = "Glock17.png";
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


void ItemGlock17::discard()
{
}









