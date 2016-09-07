//
//  InventoryData.hpp
//  DeadCreator
//
//  Created by mac on 2016. 9. 2..
//
//

#pragma once

#include <map>

#include "Types.hpp"

namespace realtrick
{
    namespace client
    {
        
        class ItemBase;
        class HumanBase;
        
        class InventoryData
        {
            
        public:
            
            explicit InventoryData(HumanBase* owner);
            virtual ~InventoryData() = default;
            
            int addItem(ItemBase* item);
            void eraseItem(int index);
            
            int getMaxItemSlot() const { return _maxItems; }
            ItemBase* getItem(int slot) { return _items[slot]; }
            ItemBase* getItemType(EntityType type);
            
            void setItemAmount(EntityType type, int amount);
            int getItemAmount(EntityType type);
            
			const std::vector<ItemBase*>& getItemLists() const { return _items; }

        private:
            
            int getClosestSlot() const;
            int getSameItemSlot(int type) const;
            
        private:
            
            HumanBase* _owner;
            std::vector<ItemBase*> _items;
            
            int _maxItems;
            
        };
        
    }
}
