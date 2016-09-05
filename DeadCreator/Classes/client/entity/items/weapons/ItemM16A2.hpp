//
//  ItemM16A2.hpp
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
        
        class ItemM16A2 : public WeaponBase
        {
            
        public:
            
            explicit ItemM16A2(Game* game);
            virtual ~ItemM16A2();
            
            static ItemM16A2* create(Game* game);
            
            virtual ItemBase* clone() const override;
            virtual void inWeapon() override;
            virtual void outWeapon() override;
            virtual void discard() override;
            virtual int getReloadedBulletOnce() override { return getMaxRounds() - getNumOfLeftRounds(); }
            virtual void dropCartiridges() override;
            
        };
        
    }
}









