//
//  WeaponView.hpp
//  DeadCreator
//
//  Created by mac on 2016. 9. 6..
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
        
        class WeaponView : public cocos2d::ui::Button
        {
            
        public:
            
            explicit WeaponView(Game* game);
            virtual ~WeaponView();
            
            static WeaponView* create(Game* game);
            virtual bool init() override;
            
            void syncWeaponView(InventoryData* data);
            
        private:
            
            Game* _game;
            cocos2d::ui::Text* _numOfBullets;
            
        };
        
    }
}









