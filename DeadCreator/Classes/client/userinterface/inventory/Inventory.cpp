//
//  Inventory.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 2. 20..
//
//

#include "Inventory.hpp"
#include "ItemSlot.hpp"
#include "ParamLoader.hpp"
#include "AnimatedButton.hpp"
#include "Game.hpp"
#include "ItemBase.hpp"
using namespace realtrick::client;
using namespace cocos2d;


Inventory::Inventory(Game* game) :
_game(game),
_backgroundView(nullptr),
_itemView(nullptr),
_slotSize(Size(Prm.getValueAsFloat("slotWidth"), Prm.getValueAsFloat("slotHeight"))),
_numOfSlotX(Prm.getValueAsInt("numOfSlotX")),
_numOfSlotY(Prm.getValueAsInt("numOfSlotY"))
{
}


Inventory::~Inventory()
{
    
}


Inventory* Inventory::create(Game* game)
{
    Inventory* ret = new (std::nothrow) Inventory(game);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool Inventory::init()
{
    if ( !Node::init() )
        return false;
    
    _backgroundView = Sprite::create("inventory_background_view2.png");
    _backgroundView->setVisible(false);
    addChild(_backgroundView);
    
    _slots.resize(_numOfSlotY * _numOfSlotX);
    
    for(int i = 0 ; i < _numOfSlotY * _numOfSlotX ; ++ i)
    {
        int x = i % _numOfSlotX;
        int y = i / _numOfSlotX;
        
        ItemSlot* slot = ItemSlot::create(_game, this, "inventory_slot_n.png", "inventory_slot_s.png", ui::Widget::TextureResType::LOCAL);
        slot->setPosition(Vec2((INVENTORY_PAD * (x + 1)) + (x * _slotSize.width) + _slotSize.width / 2,
                               (INVENTORY_PAD * (y + 1)) + (y * _slotSize.height) + _slotSize.height / 2 + 30));
        slot->setSlotSize(_slotSize);
        slot->addEventListener([&](Ref* ref, ui::CheckBox::EventType type)
                               {
                                   ItemSlot* self = (ItemSlot*)ref;
                                   int ownIndex = self->getTag();
                                   if ( type == ui::CheckBox::EventType::SELECTED )
                                   {
                                       _disableOtherButtons(ownIndex);
                                       self->enableSelected();
                                   }
                                   else if ( type == ui::CheckBox::EventType::UNSELECTED )
                                   {
                                       self->disableSelected();
                                   }
                                   
                               });
        slot->setTag(i);
        _backgroundView->addChild(slot);
        
        _slots[i] = slot;
    }
    
    return true;
}


int Inventory::getSameItemSlotIndex(ItemBase* item) const
{
    for(int i = 0 ; i < _numOfSlotY * _numOfSlotX ; ++ i)
    {
        if ( _slots[i]->isExistItem() &&
            _slots[i]->getItemPtr()->getEntityType() == item->getEntityType() )
        {
            return i;
        }
    }
    return INVALID_SLOT;
}


void Inventory::pushItem(ItemBase *item)
{
    if ( isMasked(item->getFamilyMask(), FamilyMask::WEAPON_BASE) )
    {
        // 무기 종류면 빈곳을 찾는다.
        int emptySlot = _getClosestEmptyIndex();
        _slots[emptySlot]->setItem(item);
    }
    else
    {
        // 무기류가 아니면 같은곳을 찾는다.
        // 없으면 빈곳을 찾는다.
        int idx = getSameItemSlotIndex(item);
        if ( idx != INVALID_SLOT )
        {
            // 같은 종류가 있는 슬롯을 발견했다.
            // 개수를 더한다.
            _slots[idx]->setItemAmount(_slots[idx]->getItemAmount() + item->getAmount());
        }
        else
        {
            // 같은 종류가 없다. 개수를 표시한다.
            int emptySlot = _getClosestEmptyIndex();
            _slots[emptySlot]->setItem(item);
            _slots[emptySlot]->setItemAmount(item->getAmount());
        }
    }
}


void Inventory::_disableOtherButtons(int ownIndex)
{
    for(int i = 0 ; i < _numOfSlotY * _numOfSlotX ; ++ i)
    {
        if ( i != ownIndex ) _slots[i]->disableSelected();
        else _slots[i]->setSelected(false);
    }
}


void Inventory::disableOtherEquipedItem(int ownIndex)
{
    for(int i = 0 ; i < _numOfSlotY * _numOfSlotX ; ++ i)
    {
        if ( i != ownIndex ) _slots[i]->disableEquippedItem();
    }
}


int Inventory::_getClosestEmptyIndex() const
{
    for(int i = 0 ; _numOfSlotX * _numOfSlotY ; ++ i)
    {
        if ( !_slots[i]->isExistItem() ) return i;
    }
    return INVALID_SLOT;
}


void Inventory::open()
{
    _backgroundView->setVisible(true);
}


void Inventory::close()
{
    _backgroundView->setVisible(false);
    for(int i = 0 ; i < _numOfSlotY * _numOfSlotX ; ++ i)
    {
        _slots[i]->disableSelected();
    }
}


int Inventory::getItemAmount(ItemBase* item) const
{
    for(int i = 0 ; i < _numOfSlotY * _numOfSlotX ; ++ i)
    {
        if ( _slots[i]->isExistItem() &&
            item->getEntityType() == _slots[i]->getItemPtr()->getEntityType() )
        {
            return _slots[i]->getItemAmount();
        }
    }
    
    return 0;
}

// 주의: 엔티티타입이 아이템이 아닐수도 있음.
int Inventory::getItemAmount(EntityType type) const
{
    for(int i = 0 ; i < _numOfSlotY * _numOfSlotX ; ++ i)
    {
        if ( _slots[i]->isExistItem() && type == _slots[i]->getItemPtr()->getEntityType() )
        {
            return _slots[i]->getItemAmount();
        }
    }
    
    return 0;
}


bool Inventory::isExistItem(ItemBase* item)
{
    for(int i = 0 ; i < _numOfSlotY * _numOfSlotX ; ++ i)
    {
        if ( _slots[i]->isExistItem() && item->getEntityType() == _slots[i]->getItemPtr()->getEntityType() )
        {
            return true;
        }
    }
    
    return false;
}


void Inventory::setItemAmount(ItemBase* item, int amount)
{
    if ( !isExistItem(item) ) return ;
    
    if ( !isMasked(item->getFamilyMask(), FamilyMask::WEAPON_BASE) )
    {
        int idx = getSameItemSlotIndex(item);
        if ( idx != INVALID_SLOT )
        {
            _slots[idx]->setItemAmount(amount);
        }
        else
        {
            _game->addLog("<Inventory::setItemAmount> invalid slot index");
        }
    }
    else
    {
        _game->addLog("<Inventory::setItemAmount> item is weapon.");
    }
}









