//
//  UiLayer.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 3. 20..
//
//

#pragma once

#include <chrono>
#include <bitset>

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
        class CrossHair;
        
        class UiLayer : public cocos2d::Node
        {
            
        public:
            
            explicit UiLayer(Game* game);
            virtual ~UiLayer();
            
            virtual bool init() override;
            virtual void update(float dt) override;
            static UiLayer* create(Game* game);
            
            Inventory* getInventory() const { return _inventory; }
            WeaponStatus* getWeaponStatus() const { return _weaponStatus; }
            
            void runCrossHairEffect(const std::string& name);
            void displayText(const std::string& text);
            
        private:
            
            Game* _game;
            cocos2d::Size _winSize;
        
            Inventory* _inventory;
            cocos2d::ui::CheckBox* _inventorySwitch;
            WeaponStatus* _weaponStatus;
            cocos2d::Node* _inGameUiLayer;
            CrossHair* _crossHair;
            HpBar* _hpBar;
            InfoSystem* _infoSystem;
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
            
            JoystickEx* _moveJoystick;
            JoystickEx* _attackJoystick;
            CircularBezel* _bezel;
            
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC )
            
            enum InputMask { UP = 0, DOWN, LEFT, RIGHT, RUNNING };
            std::bitset<5> _inputMask;
            cocos2d::Vec2 _mouseDirection = cocos2d::Vec2::UNIT_X;
            bool _isInputMaskDirty = false;
            
#endif
            
        };
        
    }
}









