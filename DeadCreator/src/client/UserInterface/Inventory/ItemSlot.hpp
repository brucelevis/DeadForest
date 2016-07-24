//
//  ItemSlot.hpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 2. 20..
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Inventory.hpp"
#include "ItemBase.hpp"

namespace realtrick
{
    
    class Inventory;
    class GameManager;
    
    namespace userinterface
    {
        
        class AnimatedButton;
        class ItemExtensionButtons;
        
        
        class ItemSlot : public cocos2d::ui::CheckBox
        {
            
        public:
            
            static ItemSlot* create(GameManager* mgr, Inventory* owner, const char* normal, const char* selected, cocos2d::ui::Widget::TextureResType texType = cocos2d::ui::Widget::TextureResType::LOCAL);
    
            void setItem(ItemBase* item);
            
            void throwItem();
            
            ItemBase* getItemPtr() { return _item; }
            
            cocos2d::Size getSlotSize() const { return _slotSize; }
            
            void setSlotSize(const cocos2d::Size& size);
            
            void disableSelected();
            
            void enableSelected();
            
            bool isEquippedItem() { return _isEquippedItem; }
            
            void enableEquippedItem();
            
            void disableEquippedItem();
            
            bool isExistItem() const { return (_item ? true : false); }
            
            Inventory* getInventory() const { return _owner; }
            
            int getItemAmount() const { return isExistItem() ? _item->getAmount() : -1; }
            
            void setItemAmount(int amount);
            
            void useItem();
            
            void releaseWeapon();
            
            void consumeItem();
            
        private:
            
            bool init(Inventory* owner, const char* normal, const char* selected, cocos2d::ui::Widget::TextureResType texType);
            
            explicit ItemSlot(GameManager* mgr);
            
            virtual ~ItemSlot();
            
        private:
            
            GameManager*                                                                    _gameMgr;
            
            Inventory*                                                                      _owner;
            
            ItemBase*                                                                       _item;
            
            cocos2d::Sprite*                                                                _itemImage;
            
            cocos2d::Sprite*                                                                _equippedImage;
            
            cocos2d::Size                                                                   _slotSize;
            
            ItemExtensionButtons*                                                           _extensionButtons;
            
            bool                                                                            _isEquippedItem;
            
            cocos2d::ui::Text*                                                              _numOfItems;
            
        };
        
    }
}









