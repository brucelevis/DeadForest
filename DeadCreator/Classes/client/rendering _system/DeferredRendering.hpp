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
        
        class RenderingSystem;
        class RenderTarget;
        class EntityBase;
        
        class DeferredRendering : public cocos2d::Node
        {
            
        public:
            
            explicit DeferredRendering(RenderingSystem* sys) :
            _renderingSystem(sys),
            _center(cocos2d::Vec2(0.0f, 0.0f)),
            _resolution(cocos2d::Vec2(0.0f, 0.0f)),
            _time(0.0f)
            {}
            
            virtual ~DeferredRendering() = default;
            
            static DeferredRendering* create(RenderingSystem* sys, const cocos2d::Size& size, const std::string& frag);
            bool init(const cocos2d::Size& size, const std::string& frag);
          
            virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
            virtual void setPosition(const cocos2d::Vec2& pos) override;
            virtual void update(float dt) override;
            
            void addNode(const std::string& renderTargetName, EntityBase* node, int zOrder = 0);
            
        private:
            
            void loadShader(const std::string &frag);
            void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);
            
        private:
            
            RenderingSystem* _renderingSystem;
            
            cocos2d::Size _size;
            std::string _fragFileName;
            cocos2d::Vec2 _center;
            cocos2d::Vec2 _resolution;
            float _time;
            cocos2d::CustomCommand _customCommand;
            
            std::map<std::string, RenderTarget*> _renderTargets;
            
        };
        
    }
}









