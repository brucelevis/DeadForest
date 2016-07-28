//
//  EntityRainDrop.hpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 3. 5..
//
//

#pragma once

#include "AnimatedFiniteEntity.hpp"

namespace realtrick
{
    namespace client
    {
        
        class EntityRainDrop : public AnimatedFiniteEntity
        {
            
        public:
            
            static EntityRainDrop* create(GameManager* mgr,
                                          const std::vector<std::string>& frames,
                                          float deathTime,
                                          cocos2d::ui::Widget::TextureResType type = cocos2d::ui::Widget::TextureResType::LOCAL);
            
            EntityRainDrop(GameManager* mgr);
            
            EntityRainDrop(const EntityRainDrop& rhs);
            
            virtual ~EntityRainDrop();
            
            AnimatedFiniteEntity* clone() const override;
            
            virtual bool handleMessage(const Telegram& msg) override;
            
            bool init(const std::vector<std::string> frames, float deathTime, cocos2d::ui::Widget::TextureResType type) override;
            
        private:
            
            cocos2d::Size           _winSize;
            
        };
        
    }
}









