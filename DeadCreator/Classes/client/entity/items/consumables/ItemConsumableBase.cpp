//
//  ItemConsumableBase.cpp
//  DeadCreator
//
//  Created by mac on 2016. 10. 21..
//
//

#include "ItemConsumableBase.hpp"
using namespace cocos2d;
using namespace realtrick::client;


ItemConsumableBase::ItemConsumableBase(Game* game) : ItemBase(game)
{
    ADD_FAMILY_MASK(_familyMask, CONSUMABLE_BASE);
}


ItemConsumableBase::ItemConsumableBase(const ItemConsumableBase& rhs) : ItemBase(rhs)
{
}


ItemConsumableBase::~ItemConsumableBase()
{
}


void ItemConsumableBase::use()
{
}
