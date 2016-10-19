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
        
        class Game;
        class HpBar;
        class JoystickEx;
        class CircularBezel;
        class InfoSystem;
        class CrossHair;
        class InventoryView;
        class WeaponView;
        class InventoryData;
        class CountdownTimerView;
        
        class UiLayer : public cocos2d::Node
        {
            
        public:
            
            explicit UiLayer(Game* game);
            virtual ~UiLayer();
            
            virtual bool init() override;
            virtual void update(float dt) override;
            static UiLayer* create(Game* game);
            
            void setHitPoint(float h);
            void runCrossHairEffect(const std::string& name);
            void setVisibleCrossHair(bool visible);
            void displayText(const std::string& text);
            void syncItemView(InventoryData* data);
            void syncWeaponView(InventoryData* data);
            
            void setCountdownTimer(unsigned int seconds);
            void addCountdownTimer(unsigned int seconds);
            void subtractCountdownTimer(unsigned int seconds);
            const int getCountdownTimer() const;
            
        private:
            
            bool isContainsUiLayer(const cocos2d::Vec2& p);
            bool isContainsScreenRect(const cocos2d::Vec2& p);
            
        private:
            
            Game* _game;
            cocos2d::Size _winSize;
        
            cocos2d::Node* _inGameUiLayer;
            CrossHair* _crossHair;
            HpBar* _hpBar;
            InfoSystem* _infoSystem;
            InventoryView* _inventoryView;
            WeaponView* _weaponView;
            cocos2d::ui::Button* _inventoryButton;
            CountdownTimerView* _countdownTimerView;
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
            
            JoystickEx* _moveJoystick;
            JoystickEx* _attackJoystick;
            CircularBezel* _bezel;
            
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC )
            
            enum InputMask { UP = 0, DOWN, LEFT, RIGHT };
            std::bitset<5> _inputMask;
            
            bool _isMoveMaskDirty = false;
            bool _isShiftButtonPressed = false;
            
#endif
            
        };
        
    }
}









