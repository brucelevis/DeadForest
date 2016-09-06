//
//  InventoryData.cpp
//  DeadCreator
//
//  Created by mac on 2016. 9. 2..
//
//

#include "InventoryData.hpp"
#include "ItemBase.hpp"
using namespace realtrick::client;


InventoryData::InventoryData(HumanBase* owner) :
_owner(owner),
_maxItems(12)
{
    _items.resize(_maxItems, nullptr);
}


int InventoryData::addItem(ItemBase* item)
{
    // cocos2d::Node를 상속받은 item 객체는 사라질것이기 때문에
    // 데이터만을 가지고있는 복사본을 활용한다.
    auto clonedItem = item->clone();
    
    int slot = getClosestSlot();
    if ( slot == -1 ) return -1;
    
    if ( isMasked(item->getFamilyMask(), FamilyMask::WEAPON_BASE) )
    {
        // 무기 종류면 가까운 빈 슬롯에 넣는다.
        _items[slot] = clonedItem;
    }
    else
    {
        // 무기류가 아니면 같은곳을 찾는다.
        int sameSlot = getSameItemSlot(clonedItem->getEntityType());
        if ( sameSlot != -1 )
        {
            auto originItem = _items[sameSlot];
            originItem->setAmount(originItem->getAmount() + clonedItem->getAmount());
        }
        else
        {
            // 같은 종류가 없으면 빈 곳에 넣는다.
            _items[slot] = clonedItem;
        }
    }

    return slot;
}


void InventoryData::eraseItem(int index)
{
    if ( !_items[index] )
    {
        throw std::runtime_error("<InventoryData::eraseItem> item is not exist");
    }
    else
    {
        CC_SAFE_DELETE(_items[index]);
    }
}


int InventoryData::getClosestSlot() const
{
    int ret = 0;
    for(auto& item : _items)
    {
        if ( !item ) return ret;
        ret ++;
    }
    return -1;
}


int InventoryData::getSameItemSlot(int type) const
{
    int ret = 0;
    for(auto& item : _items)
    {
        if ( item && item->getEntityType() == type ) return ret;
        ret ++;
    }
    return -1;
}


ItemBase* InventoryData::getItemType(EntityType type)
{
    for(const auto& item : _items)
    {
        if ( item && item->getEntityType() == type ) return item;
    }
    return nullptr;
}


void InventoryData::setItemAmount(EntityType type, int amount)
{
    auto item = getItemType(type);
    if ( item )
    {
        int slot = getSameItemSlot(type);
        if ( amount == 0 ) eraseItem(slot);
        else item->setAmount(amount);
    }
}


int InventoryData::getItemAmount(EntityType type)
{
    auto item = getItemType(type);
    if ( item ) return item->getAmount();
    return -1;
}









