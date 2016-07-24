//
//  ItemM16A2.hpp
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
    
    class ItemM16A2 : public WeaponBase
    {
        
    public:
        
        static ItemM16A2* create(GameManager* mgr);
        
        virtual ItemBase* clone() const override;
        
        virtual void inWeapon() override;
        
        virtual void outWeapon() override;
        
        virtual void attack() override;
        
        virtual void discard() override;
        
        virtual int getReloadedBulletOnce() override { return getMaxRounds() - getNumOfLeftRounds(); }
        
    private:
        
        ItemM16A2(GameManager* mgr);
        
        virtual ~ItemM16A2();
        
    };
    
}




