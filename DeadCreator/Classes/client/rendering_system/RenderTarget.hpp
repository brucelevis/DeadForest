//
//  RenderTarget.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 18..
//
//

#pragma once

#include "cocos2d.h"

namespace realtrick
{
    namespace client
    {
        
        class Camera2D;
        
        class RenderTarget : public cocos2d::Node
        {
            
        public:
            
            virtual ~RenderTarget();
            static RenderTarget* create(const cocos2d::Size& textureSize);
            bool init(const cocos2d::Size& textureSize);
            void transform(const cocos2d::Vec2& scale, const cocos2d::Vec2& translate);
            cocos2d::Texture2D* getTexture();
            
            void enableNormal();
            void disableNormal();
            
        private:
            
            cocos2d::RenderTexture* _renderTexture;
            
        };
        
    }
}









