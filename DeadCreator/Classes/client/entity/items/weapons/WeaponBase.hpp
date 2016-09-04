//
//  WeaponBase.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 1. 13..
//
//

#pragma once

#include "ItemBase.hpp"

namespace realtrick
{
    namespace client
    {
        
        class HumanBase;
        
        class WeaponBase : public ItemBase
        {
            
        public:
            
            explicit WeaponBase(Game* game);
            WeaponBase(const WeaponBase& rhs);
            virtual ~WeaponBase();
            
            int getMaxRounds() const { return _maxRounds; }
            int getNumOfLeftRounds() const { return _numOfLeftRounds; }
            void setNumOfLeftRounds(int rounds) { _numOfLeftRounds = rounds; }
            int getDamage() const { return _damage; }
            float getRange() const { return _range; }
            float getDelay() const { return _delay; }
            bool isReadyToAttack() const { return _isReadyToAttack; }
            void enableReadyToAttack(bool enable) { _isReadyToAttack = enable; }
            EntityType getBulletType() const { return _bulletType; }
            int getReservedBullets() const { return _reservedBullet; }
            void setReservecBullets(int num) { _reservedBullet = num; }
            virtual int getReloadedBulletOnce() { return 0; }
            
            virtual void attack() final;
            virtual void reload() final;
            virtual void use() override final;
            virtual void releaseWeapon() override final;
            
            virtual void inWeapon() = 0;
            virtual void outWeapon() = 0;
            virtual ItemBase* clone() const override = 0;
            virtual void discard() override = 0;
            
        protected:
            
            void setDamage(int dam) { _damage = dam; }
            void setRange(float range) { _range = range; }
            void setDelay(float delay) { _delay = delay; }
            void setMaxRounds(int rounds) { _maxRounds = rounds; }
            void setBulletType(EntityType bulletType) { _bulletType = bulletType; }
            
        protected:
            
            int                         _numOfLeftRounds;
            int                         _maxRounds;
            int                         _damage;
            float                       _range;
            float                       _delay;
            bool                        _isReadyToAttack;
            EntityType                  _bulletType;
            int                         _reservedBullet;
            
        };
        
    }
}









