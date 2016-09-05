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
    _damage = 100;
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









