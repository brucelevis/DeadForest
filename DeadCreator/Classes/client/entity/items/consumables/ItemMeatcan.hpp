//
//  ItemMeatcan.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 10. 21..
//
//


#include "ItemConsumableBase.hpp"
#include "ui/CocosGUI.h"

namespace realtrick
{
    namespace client
    {
        
        class ItemMeatcan : public ItemConsumableBase
        {
            
        public:
            
            explicit ItemMeatcan(Game* game);
            ItemMeatcan(const ItemMeatcan& rhs);
            virtual ~ItemMeatcan();
            
            static ItemMeatcan* create(Game* game);
            
            virtual ItemBase* clone() const override;
            virtual void use() override;
            virtual void discard() override;
            
        };
        
    }
}
