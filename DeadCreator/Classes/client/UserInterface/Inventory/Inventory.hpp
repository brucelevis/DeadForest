//
//  Inventory.hpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 2. 20..
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "EntityType.hpp"

#define INVENTORY_PAD 20.0f

namespace realtrick
{
    
    class ItemBase;
    class GameManager;
    
    namespace userinterface
    {
        
        class ItemSlot;
        class AnimatedButton;
        
        class Inventory : public cocos2d::Node
        {
            
            enum { INVALID_SLOT = -1 };
            
        public:
            
            static Inventory* create(GameManager* mgr);
            
            void pushItem(ItemBase* item);
            
            bool isFull() const { return (_getClosestEmptyIndex() == INVALID_SLOT); }
            
            cocos2d::Sprite* getBackgroundView() const { return _backgroundView; }
            
            void open();
            
            void close();
            
            bool isOpen() const { return _backgroundView->isVisible(); }
            
            cocos2d::Size getSlotSize() const { return _slotSize; }
            
            int getItemAmount(ItemBase* item) const;
            
            int getItemAmount(EntityType type) const;
            
            void setItemAmount(ItemBase* item, int amount);
            
            void disableOtherEquipedItem(int idx);
            
            bool isExistItem(ItemBase* item);
            
            int getSameItemSlotIndex(ItemBase* item) const ;
            
        private:
            
            explicit Inventory(GameManager* mgr);
            
            virtual ~Inventory();
            
            bool init() override;
            
            void _disableOtherButtons(int ownIndex);
            
            int _getClosestEmptyIndex() const;
            
        private:
            
            GameManager*                            _gameMgr;
            
            cocos2d::Sprite*                        _backgroundView;
            
            cocos2d::Node*                          _itemView;
            
            std::vector<ItemSlot*>                  _slots;
            
            cocos2d::Size                           _slotSize;
            
            int                                     _numOfSlotX;
            
            int                                     _numOfSlotY;
            
            bool                                    _isFull;
            
        };
        
    }
}









