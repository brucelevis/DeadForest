//
//  ItemAxe.hpp
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
        
        class ItemAxe : public WeaponBase
        {
            
        public:
            
            explicit ItemAxe(Game* game);
            virtual ~ItemAxe();
            
            static ItemAxe* create(Game* game);
            virtual ItemBase* clone() const override;
            virtual void inWeapon() override;
            virtual void outWeapon() override;
            virtual void discard() override;
            virtual void attackImpl() override;
            
        };
        
    }
}









