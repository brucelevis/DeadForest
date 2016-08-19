//
//  EffectSprite.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 19..
//
//

#pragma once

#include "cocos2d.h"

namespace realtrick
{
    namespace client
    {
        
        class EffectBase;
        
        class EffectSprite : public cocos2d::Sprite
        {
            
        public:
            
            static EffectSprite* create();
            static EffectSprite* create(const std::string& filename);
            
            void setEffect(EffectBase* effect);
            void addEffect(EffectBase* effect, ssize_t order);
            void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
            
        protected:
            
            EffectSprite();
            ~EffectSprite();
            
            std::vector<std::tuple<ssize_t, EffectBase*, cocos2d::QuadCommand>> _effects;
            EffectBase* _defaultEffect;
            
        };
        
    }
}









