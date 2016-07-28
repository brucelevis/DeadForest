//
//  ItemM1897.hpp
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
        
        class ItemM1897 : public WeaponBase
        {
            
        public:
            
            explicit ItemM1897(GameManager* mgr);
            virtual ~ItemM1897();
            static ItemM1897* create(GameManager* mgr);
            
            void setNumOfShells(int num) { _numOfShells = num; }
            int getNumOfShells() const { return _numOfShells; }
            
            virtual ItemBase* clone() const override;
            virtual void inWeapon() override;
            virtual void outWeapon() override;
            virtual void attack() override;
            virtual void discard() override;
            virtual int getReloadedBulletOnce() override { return 1; }
            
        private:
            
            int _numOfShells;
            
        };
        
    }
}




