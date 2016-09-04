//
//  ItemM1897.hpp
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
        
        class ItemM1897 : public WeaponBase
        {
            
        public:
            
            explicit ItemM1897(Game* game);
            virtual ~ItemM1897();
            static ItemM1897* create(Game* game);
            
            void setNumOfShells(int num) { _numOfShells = num; }
            int getNumOfShells() const { return _numOfShells; }
            
            virtual ItemBase* clone() const override;
            virtual void inWeapon() override;
            virtual void outWeapon() override;
            virtual void discard() override;
            virtual int getReloadedBulletOnce() override { return 1; }
            
        private:
            
            int _numOfShells;
            
        };
        
    }
}




