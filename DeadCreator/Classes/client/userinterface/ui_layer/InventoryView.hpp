//
//  InventoryView.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 9. 2..
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"


namespace realtrick
{
    namespace client
    {
        
        class Game;
        class ItemBase;
        class InventoryData;
        
        class InventoryView : public cocos2d::Node
        {
            
        public:
            
            explicit InventoryView(Game* game);
            virtual ~InventoryView();
            
            static InventoryView* create(Game* game);
            virtual bool init() override;
            
            void syncItemView(InventoryData* data);
            Game* getGame() const { return _game; }
            
        private:
            
            Game* _game;
            
            cocos2d::Sprite* _background;
            std::vector<cocos2d::ui::Button*> _slots;
            cocos2d::Sprite* _infoView;
            
        };
        
    }
}









