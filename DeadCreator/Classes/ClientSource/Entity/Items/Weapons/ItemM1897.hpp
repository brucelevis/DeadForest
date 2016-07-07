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
    
    class ItemM1897 : public WeaponBase
    {
        
    public:
        
        static ItemM1897* create(GameManager* mgr,
                                 const char* inGameImage_n, const char* inGameImage_s, const char* inSlotImage,
                                 cocos2d::ui::Widget::TextureResType texResType = cocos2d::ui::Widget::TextureResType::LOCAL);
        
        void setNumOfShells(int num) { _numOfShells = num; }
        
        int getNumOfShells() const { return _numOfShells; }
        
        virtual ItemBase* clone() const override;
        
        virtual void inWeapon() override;
        
        virtual void outWeapon() override;
        
        virtual void attack() override;
        
        virtual void discard() override;
        
        virtual int getReloadedBulletOnce() override { return 1; }
        
    private:
        
        int         _numOfShells;
        
    private:
        
        ItemM1897(GameManager* mgr);
        
        virtual ~ItemM1897();
        
    };
    
}




