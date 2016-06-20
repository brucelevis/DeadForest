//
//  EntityEmptyCartridge.hpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 3. 5..
//
//

#pragma once

#include "AnimatedFiniteEntity.hpp"

namespace realtrick
{
    
    class EntityEmptyCartridge : public AnimatedFiniteEntity
    {
        
    public:
        
        static EntityEmptyCartridge* create(GameManager* mgr,
                                            const std::vector<std::string>& frames,
                                            float deathTime,
                                            cocos2d::ui::Widget::TextureResType type = cocos2d::ui::Widget::TextureResType::LOCAL);
        
        EntityEmptyCartridge(GameManager* mgr);
        
        EntityEmptyCartridge(const EntityEmptyCartridge& rhs);
        
        virtual ~EntityEmptyCartridge();
        
        AnimatedFiniteEntity* clone() const override;
        
    };
    
}









