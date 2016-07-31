//
//  RenderTarget.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 3. 3..
//
//

#pragma once

#include "cocos2d.h"

namespace realtrick
{
    
    namespace client
    {
        
        class Game;
        class OcculusionBuilder;
        class EffectSprite;
        class LightEffect;
        class EntityBase;
        
        class RenderTarget : public cocos2d::Node
        {
            
        public:
            
            explicit RenderTarget(Game* game);
            virtual ~RenderTarget();
            
            bool init() override;
            static RenderTarget* create(Game* game);
            
            cocos2d::Texture2D* getNormalTex() const;
            cocos2d::Texture2D* getStaticTex() const;
            cocos2d::Texture2D* getDynamicTex() const;
            cocos2d::Texture2D* getOcculusionTex() const;
            
            void setZoom(float ratio);
            void pauseGame() { _isPaused = true; }
            void resumeGame() { _isPaused = false; }
            
            void addDynamicEntity(EntityBase* entity, int zOrder);
            void addStaticEntity(cocos2d::Node* entity, int zOrder);
            
        private:
            
            Game*                _game;
            cocos2d::Size               _winSize;
            
            cocos2d::Node*              _staticTarget;
            cocos2d::Node*              _dynamicTarget;
            
            cocos2d::RenderTexture*     _normalTex;
            cocos2d::RenderTexture*     _staticTex;
            cocos2d::RenderTexture*     _dynamicTex;
            cocos2d::RenderTexture*     _occulusionTex;
            OcculusionBuilder*          _occulusionBuilder;
            
            cocos2d::Sprite*            _normalBg;
            
            EffectSprite*               _deferredRenderTarget;
            LightEffect*                _effect;
            
            bool                        _isPaused;
            
        private:
            
            virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;
            
        };
        
    }
}









