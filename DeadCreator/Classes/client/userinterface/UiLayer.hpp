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
        class GameManager;
        class AimingNode;
        class Inventory;
        class WeaponStatus;
        class HpBar;
        class InfoSystem;
        
        class UiLayer : public cocos2d::Node
        {
            
        public:
            
            explicit UiLayer(GameManager* mgr);
            virtual ~UiLayer();
            
            virtual bool init() override;
            static UiLayer* create(GameManager* mgr);
            
            Inventory* getInventory() const { return _inventory; }
            WeaponStatus* getWeaponStatus() const { return _weaponStatus; }
            AimingNode* getAimingNode() const { return _aimingNode; }
            
        private:
            
            GameManager* _gameMgr;
            cocos2d::Size _winSize;
            JoystickEx* _moveJoystick;
            JoystickEx* _attackJoystick;
            CircularBezel* _bezel;
            Inventory* _inventory;
            cocos2d::ui::CheckBox* _inventorySwitch;
            cocos2d::Node* _inGameUIRoot;
            WeaponStatus* _weaponStatus;
            AimingNode* _aimingNode;
            HpBar* _hpBar;
            
        };
        
    }
}







