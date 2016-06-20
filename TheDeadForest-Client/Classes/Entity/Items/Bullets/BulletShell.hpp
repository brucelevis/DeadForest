//
//  BulletShell.hpp
//  TheDeadForest
//
//  Created by mac on 2016. 2. 3..
//
//

#pragma once

#include "BulletBase.hpp"

namespace realtrick
{
    
    class BulletShell : public BulletBase
    {
        
    public:
        
        static BulletShell* create(GameManager* mgr,
                                   const char* inGameImage_n, const char* inGameImage_s, const char* inSlotImage,
                                   cocos2d::ui::Widget::TextureResType texResType = cocos2d::ui::Widget::TextureResType::LOCAL);
        
        virtual BulletShell* clone() const override;
        
        virtual void discard() override;
        
    private:
        
        BulletShell(GameManager* mgr);
        
        virtual ~BulletShell();
        
    };
    
}