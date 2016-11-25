//
//  ItemMeatcan.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 10. 21..
//
//

#include "ItemMeatcan.hpp"
using namespace cocos2d;
using namespace realtrick::client;


ItemMeatcan::ItemMeatcan(Game* game) : ItemConsumableBase(game)
{
    setEntityType(CONSUMABLE_MEATCAN);
    _spriteName = "MeatCan.png";
    _amount = 1;
}


ItemMeatcan::ItemMeatcan(const ItemMeatcan& rhs) : ItemConsumableBase(rhs)
{
}


ItemMeatcan::~ItemMeatcan()
{
}


ItemMeatcan* ItemMeatcan::create(Game* game)
{
    ItemMeatcan* ret = new (std::nothrow)ItemMeatcan(game);
    if( ret && ret->init("MeatCan.png", "MeatCan.png", "MeatCan.png", cocos2d::ui::Widget::TextureResType::PLIST))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


ItemBase* ItemMeatcan::clone() const
{
    return new ItemMeatcan(*this);
}


void ItemMeatcan::use()
{
}


void ItemMeatcan::discard()
{
}









