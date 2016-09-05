//
//  EntityPlayer.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 2..
//
//

#pragma once

#include "HumanBase.hpp"

namespace realtrick
{
    namespace client
    {
        
        class WeaponBase;
        class InventoryData;
        class WeaponStatus;
        class UiLayer;
        
        class EntityPlayer : public HumanBase
        {
            
        public:
            
            explicit EntityPlayer(Game* game);
            virtual ~EntityPlayer();
            
            virtual bool init() override;
            static EntityPlayer* create(Game* game);

            virtual bool handleMessage(const Telegram& msg) override;
            virtual bool isIntersectOther(const cocos2d::Vec2& futurePosition, EntityBase* other) override;
            
            WeaponBase* getEquipedWeapon() const { return _equipedWeapon; }
            void setEquipedWeapon(WeaponBase* newWeapon) { _equipedWeapon = newWeapon; }
            
            void setUserNickName(const std::string& name) { _userNickName = name; }
            std::string getUserNickName() const { return _userNickName; }
            
            void reload();
            
            virtual void update(float dt) override;
            virtual void suicide() override;
            
            bool addItem(ItemBase* item);
            
            void setUiLayer(UiLayer* uiLayer) { _uiLayer = uiLayer; }
            
        protected:
            
            UiLayer*                        _uiLayer;
            InventoryData*                  _inventoryData;
            WeaponBase*                     _equipedWeapon;
            std::string                     _userNickName;
            
        };
        
    }
}









