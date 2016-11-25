//
//  ItemBandage.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 10. 21..
//
//

#pragma once

#include "ItemConsumableBase.hpp"
#include "ui/CocosGUI.h"


namespace realtrick
{
    namespace client
    {
        
        class ItemBandage : public ItemConsumableBase
        {
            
        public:
            
            explicit ItemBandage(Game* game);
            ItemBandage(const ItemBandage& rhs);
            virtual ~ItemBandage();
            
            static ItemBandage* create(Game* game);
            
            virtual ItemBase* clone() const override;
            virtual void use() override;
            virtual void discard() override;
            
        };
        
    }
}









