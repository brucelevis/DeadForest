//
//  DeferredRendering.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 19..
//
//


#pragma once

#include "cocos2d.h"
#include "EffectSprite.hpp"

namespace realtrick
{
    namespace client
    {
        
        class RenderTarget;
        class RenderingSystem;
        class EntityBase;
        
        class DeferredRendering : public EffectSprite
        {
            
        public:
            
            DeferredRendering();
            virtual ~DeferredRendering();
            
            static DeferredRendering* create(const std::string& basicTextureName);
            virtual bool initWithFile(const std::string& basicTextureName) override;
            void prepareToRender(const cocos2d::Vec2& zoomScale, const cocos2d::Vec2& cameraPos);
            
            void addEntity(const std::string& renderTargetName, EntityBase* node, int zOrder = 0);
            
        private:
            
            std::map<std::string, RenderTarget*> _renderTargets;
            
        };
        
    }
}