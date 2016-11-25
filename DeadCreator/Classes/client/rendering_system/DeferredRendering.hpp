//
//  DeferredRendering.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 8. 19..
//
//


#pragma once

#include "cocos2d.h"
#include "EffectSprite.hpp"

namespace realtrick
{
    namespace client
    {
        
        class Game;
        class RenderingSystem;
        class EntityBase;
        class OcclusionBaker;
        
        class DeferredRendering : public EffectSprite
        {
            
        public:
            
            explicit DeferredRendering(Game* game);
            virtual ~DeferredRendering();
            
            static DeferredRendering* create(Game* game, const std::string& basicTextureName);
            virtual bool initWithFile(const std::string& basicTextureName) override;
            void prepareToRender(const cocos2d::Vec2& zoomScale, const cocos2d::Vec2& cameraPos);
            
            void addDynamicEntity(EntityBase* node, int zOrder = 0);
            void addStaticEntity(EntityBase* node, int zOrder = 0);
            
        private:
            
            Game* _game;
            
            cocos2d::Node* _dynamicEntities;
            cocos2d::Node* _staticEntities;
            
            cocos2d::RenderTexture* _dynamicTexture;
            cocos2d::RenderTexture* _staticTexture;
            cocos2d::RenderTexture* _normalTexture;
            cocos2d::RenderTexture* _occlusionTexture;
            
            OcclusionBaker* _occlusionBaker;
            
        };
        
    }
}
