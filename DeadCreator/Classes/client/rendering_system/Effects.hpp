//
//  Effects.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 19..
//
//

#pragma once

#include "EffectBase.hpp"


namespace realtrick
{
    namespace client
    {
        
        class EffectSprite;
        
        // Blur
        class EffectBlur : public EffectBase
        {
            
        public:
            
            CREATE_FUNC(EffectBlur);
            bool init(float blurRadius = 10.0f, float sampleNum = 5.0f);
            
            virtual void setTarget(EffectSprite *sprite) override;
            void setBlurRadius(float radius);
            void setBlurSampleNum(float num);
            
        protected:
            
            float _blurRadius;
            float _blurSampleNum;
        };
        
        
        // Outline
        class EffectOutline : public EffectBase
        {
            
        public:
            
            CREATE_FUNC(EffectOutline);
            bool init();
            
        };
        
        
        // Noise
        class EffectNoise : public EffectBase
        {
            
        public:
            
            CREATE_FUNC(EffectNoise);
            bool init();
            
        protected:
        
            virtual void setTarget(EffectSprite* sprite) override;
            
        };
        
        
        // deferred rendering
        class EffectDeferredRendering : public EffectBase
        {
            
        public:
            
            CREATE_FUNC(EffectDeferredRendering);
            bool init();
            
        protected:
            
            void setTarget(EffectSprite* sprite);
            
        };
    
        
    }
}









