//
//  UiLayer.hpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 3. 20..
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace realtrick
{
    
    class JoystickEx;
    class CircularBezel;
    class GameManager;
    class AimingNode;
    
    namespace userinterface
    {
        class Inventory;
        class WeaponStatus;
        class HpBar;
        class InfoSystem;
    }
    
    class UiLayer : public cocos2d::Node
    {
        
    public:
        
        static UiLayer* create(GameManager* mgr);
        
        virtual bool init() override;
        
        userinterface::Inventory* getInventory() const { return _inventory; }
        
        userinterface::WeaponStatus* getWeaponStatus() const { return _weaponStatus; }
        
        AimingNode* getAimingNode() const { return _aimingNode; }
        
        void pushMessageToBottom(const std::string& infoText);
        
        void pushMessageToTop(const std::string& infoText);
        
    private:
        
        UiLayer(GameManager* mgr);
        
        virtual ~UiLayer();
        
    private:
        
        GameManager* _gameMgr;
        
        cocos2d::Size _winSize;
        
        JoystickEx* _moveJoystick;
        
        JoystickEx* _attackJoystick;
        
        CircularBezel* _bezel;
        
        userinterface::Inventory* _inventory;
        
        cocos2d::ui::CheckBox* _inventorySwitch;
        
        cocos2d::Node* _inGameUIRoot;
        
        cocos2d::ui::Text* _locationText;
        
        userinterface::WeaponStatus* _weaponStatus;
        
        AimingNode* _aimingNode;
        
        userinterface::HpBar* _hpBar;
        
        userinterface::InfoSystem* _infoSystemTop;
        
        userinterface::InfoSystem* _infoSystemBottom;
        
    };
}









