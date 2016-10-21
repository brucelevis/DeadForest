//
//  ItemConsumableBase.hpp
//  DeadCreator
//
//  Created by mac on 2016. 10. 21..
//
//


#pragma once

#include "ItemBase.hpp"


namespace realtrick
{
    namespace client
    {
        
        class HumanBase;
        
        class ItemConsumableBase : public ItemBase
        {
            
        public:
            
            explicit ItemConsumableBase(Game* game);
            ItemConsumableBase(const ItemConsumableBase& rhs);
            virtual ~ItemConsumableBase();
            
            virtual void use() override;
            virtual ItemBase* clone() const override = 0;
            virtual void discard() override = 0;
            
        };
        
    }
}









