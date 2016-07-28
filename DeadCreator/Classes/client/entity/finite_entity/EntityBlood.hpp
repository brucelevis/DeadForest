//
//  EntityBlood.hpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 3. 2..
//
//

#pragma once

#include "FiniteEntityBase.hpp"

namespace realtrick
{
    namespace client
    {
        
    class EntityBlood : public FiniteEntityBase
    {
        
    public:
        
        static EntityBlood* create(GameManager* mgr,
                                   const std::string& fileName,
                                   float deathTime,
                                   cocos2d::ui::Widget::TextureResType type = cocos2d::ui::Widget::TextureResType::LOCAL);
        
        EntityBlood(GameManager* mgr);
        EntityBlood(const EntityBlood& rhs);
        virtual ~EntityBlood();
        FiniteEntityBase* clone() const override;
        
    };
        
    }
}









