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
            
            explicit ItemAxe(GameManager* mgr);
            virtual ~ItemAxe();
            
            static ItemAxe* create(GameManager* mgr);
            virtual ItemBase* clone() const override;
            virtual void inWeapon() override;
            virtual void outWeapon() override;
            virtual void attack() override;
            virtual void discard() override;
            
        };
        
    }
}









