//
//  ItemBandage.cpp
//  DeadCreator
//
//  Created by mac on 2016. 10. 21..
//
//

#include "ItemBandage.hpp"
using namespace cocos2d;
using namespace realtrick::client;


ItemBandage::ItemBandage(Game* game) : ItemConsumableBase(game)
{
    setEntityType(CONSUMABLE_BANDAGE);
    _spriteName = "Bandage.png";
    _amount = 1;
}


ItemBandage::ItemBandage(const ItemBandage& rhs) : ItemConsumableBase(rhs)
{
}


ItemBandage::~ItemBandage()
{
}


ItemBandage* ItemBandage::create(Game* game)
{
    ItemBandage* ret = new (std::nothrow)ItemBandage(game);
    if( ret && ret->init("Bandage.png", "Bandage.png", "Bandage.png", cocos2d::ui::Widget::TextureResType::PLIST))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


ItemBase* ItemBandage::clone() const
{
    return new ItemBandage(*this);
}


void ItemBandage::use()
{
}


void ItemBandage::discard()
{
}









