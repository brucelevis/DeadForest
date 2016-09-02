//
//  InventoryData.hpp
//  DeadCreator
//
//  Created by mac on 2016. 9. 2..
//
//

#pragma once

#include <map>

namespace realtrick
{
    namespace client
    {
        
        class ItemBase;
        class EntityPlayer;
        
        class InventoryData
        {
            
        public:
            
            explicit InventoryData(EntityPlayer* owner);
            virtual ~InventoryData() = default;
            
            bool addItem(ItemBase* item);
            void eraseItem(int index);
            
        private:
            
            int getClosestSlot() const;
            int getSameItemSlot(int type) const;
            
        private:
            
            EntityPlayer* _owner;
            std::vector<ItemBase*> _items;
            
            int _maxItems;
            
        };
        
    }
}
