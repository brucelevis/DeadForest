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
        class Inventory;
        class WeaponStatus;
        
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
            
            Inventory* getInventory() const { return _inventory; }
            WeaponStatus* getWeaponStatus() const { return _weaponStatus; }
            
            void setUserNickName(const std::string& name) { _userNickName = name; }
            std::string getUserNickName() const { return _userNickName; }
            
            virtual void update(float dt) override;
            virtual void suicide() override;
            
        protected:
            
            WeaponBase*                     _equipedWeapon;
            WeaponStatus*                   _weaponStatus;
            Inventory*                      _inventory;
            
            std::string                     _userNickName;
            
        };
        
    }
}









