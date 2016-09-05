//
//  InventoryView.hpp
//  DeadCreator
//
//  Created by mac on 2016. 9. 2..
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"


namespace realtrick
{
    namespace client
    {
        
        class ItemBase;
        class InventoryData;
        
        class InventoryView : public cocos2d::Node
        {
            
        public:
            
            static InventoryView* create();
            virtual bool init() override;
            
            void syncItemView(InventoryData* data);
            
        private:
            
            cocos2d::Sprite* _background;
            std::vector<cocos2d::ui::Button*> _slots;
            cocos2d::Sprite* _infoView;
            
        };
        
    }
}









