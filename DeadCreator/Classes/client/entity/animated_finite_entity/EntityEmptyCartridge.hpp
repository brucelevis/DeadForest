//
//  EntityEmptyCartridge.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 3. 5..
//
//

#pragma once

#include "AnimatedFiniteEntity.hpp"

namespace realtrick
{
    namespace client
    {
        
        class GameManager;
        
        class EntityEmptyCartridge : public AnimatedFiniteEntity
        {
            
        public:
            
            static EntityEmptyCartridge* create(GameManager* mgr,
                                                const std::vector<std::string>& frames,
                                                float deathTime,
                                                cocos2d::ui::Widget::TextureResType type = cocos2d::ui::Widget::TextureResType::LOCAL);
            
            explicit EntityEmptyCartridge(GameManager* mgr);
            EntityEmptyCartridge(const EntityEmptyCartridge& rhs);
            virtual ~EntityEmptyCartridge();
            AnimatedFiniteEntity* clone() const override;
            
        };
        
    }
}









