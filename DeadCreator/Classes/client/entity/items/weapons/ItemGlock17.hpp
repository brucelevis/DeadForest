//
//  ItemGlock17.hpp
//  TheDeadForest
//
//  Created by mac on 2016. 1. 13..
//
//

#pragma once

#include "WeaponBase.hpp"
#include "ui/CocosGUI.h"

namespace realtrick
{
    namespace client
    {
        
        class ItemGlock17 : public WeaponBase
        {
            
        public:
            
            explicit ItemGlock17(GameManager* mgr);
            ItemGlock17(const ItemGlock17& rhs);
            virtual ~ItemGlock17();
            
            static ItemGlock17* create(GameManager* mgr);
            
            virtual ItemBase* clone() const override;
            
            virtual void inWeapon() override;
            virtual void outWeapon() override;
            virtual void attack() override;
            virtual void discard() override;
            virtual int getReloadedBulletOnce() override { return getMaxRounds() - getNumOfLeftRounds(); }
            
        };
        
    }
}









