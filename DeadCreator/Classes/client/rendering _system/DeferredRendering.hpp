//
//  DeferredRendering.hpp
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
        
        class DeferredRendering : public cocos2d::Node
        {
            
        public:
            
            DeferredRendering();
            virtual ~DeferredRendering();
            
            static DeferredRendering* create(const cocos2d::Size& size, const std::string& frag);
            bool init(const cocos2d::Size& size, const std::string& frag);
          
            virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
            
        private:
            
            void loadShader(const std::string &frag);
            void onDraw(const cocos2d::Mat4& transform, uint32_t flags);
            
        private:
            
            cocos2d::Size _size;
            std::string _fragFileName;
            cocos2d::CustomCommand _customCommand;
          
            cocos2d::Sprite* spr;
            
        };
        
    }
}