//
//  ItemAxe.hpp
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
    
    class ItemAxe : public WeaponBase
    {
        
    public:
        
        static ItemAxe* create(GameManager* mgr);
        
        virtual ItemBase* clone() const override;
        
        virtual void inWeapon() override;
        
        virtual void outWeapon() override;
        
        virtual void attack() override;
        
        virtual void discard() override;
        
    private:
        
        ItemAxe(GameManager* mgr);
        
        virtual ~ItemAxe();
        
    };
    
}




