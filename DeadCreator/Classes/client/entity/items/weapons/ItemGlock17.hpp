//
//  ItemGlock17.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 1. 13..
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
            
            explicit ItemGlock17(Game* game);
            ItemGlock17(const ItemGlock17& rhs);
            virtual ~ItemGlock17();
            
            static ItemGlock17* create(Game* game);
            
            virtual ItemBase* clone() const override;
            
            virtual void inWeapon() override;
            virtual void outWeapon() override;

            virtual void discard() override;
            virtual int getReloadedBulletOnce() override { return getMaxRounds() - getNumOfLeftRounds(); }
            
        };
        
    }
}









