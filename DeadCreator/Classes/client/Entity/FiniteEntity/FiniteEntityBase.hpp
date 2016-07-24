//
//  FiniteEntityBase.hpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 3. 2..
//
//

#pragma once

#include "EntityBase.hpp"
#include "CocosGUI.h"

namespace realtrick
{
    
    class FiniteEntityBase : public EntityBase
    {
        
    public:
        
        FiniteEntityBase(GameManager* mgr);
        
        FiniteEntityBase(const FiniteEntityBase& rhs);
        
        virtual ~FiniteEntityBase();
        
        bool init(const std::string& inGameImagePath, float deathTime, cocos2d::ui::Widget::TextureResType type);
        
        void enableNormal(bool enable) override {}
        
        virtual FiniteEntityBase* clone() const = 0;
        
        virtual bool handleMessage(const Telegram& msg) override;
        
    protected:
        
        float               _deathTime;
        
        cocos2d::Sprite*    _inGameImage;
        
    };
    
}









