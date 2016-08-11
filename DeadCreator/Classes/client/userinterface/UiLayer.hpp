//
//  UiLayer.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 3. 20..
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace realtrick
{
    namespace client
    {
        
        class JoystickEx;
        class CircularBezel;
        class Game;
        class Inventory;
        class WeaponStatus;
        class HpBar;
        class InfoSystem;
        
        class UiLayer : public cocos2d::Node
        {
            
        public:
            
            explicit UiLayer(Game* game);
            virtual ~UiLayer();
            
            virtual bool init() override;
            static UiLayer* create(Game* game);
            
            Inventory* getInventory() const { return _inventory; }
            WeaponStatus* getWeaponStatus() const { return _weaponStatus; }
            
        private:
            
            Game* _game;
            cocos2d::Size _winSize;
            JoystickEx* _moveJoystick;
            JoystickEx* _attackJoystick;
            CircularBezel* _bezel;
            Inventory* _inventory;
            cocos2d::ui::CheckBox* _inventorySwitch;
            WeaponStatus* _weaponStatus;
            HpBar* _hpBar;
            
        };
        
    }
}









