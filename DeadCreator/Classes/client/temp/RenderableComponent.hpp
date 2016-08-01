//
//  RenderableComponent.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 31..
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "ComponentBase.hpp"


namespace realtrick
{
    namespace client
    {
        
        class GameObject2;
        class Telegram;
        
        class RenderableComponent : public ComponentBase, public cocos2d::Node
        {
            
        public:
            
            RenderableComponent(GameObject2* owner) : _owner(owner)
            {
                _type = ComponentType::RENDERABLE;
            }
            virtual ~RenderableComponent() { this->clear(); }
            
            static RenderableComponent* create(GameObject2* owner, const std::string& fileName,
                                              cocos2d::ui::Widget::TextureResType resType)
            {
                auto ret = new (std::nothrow) RenderableComponent(owner);
                if ( ret && ret->initWithFile(fileName, resType) )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool isEnableShadow() const { return _shadow->isVisible(); }
            void enableShadow(bool enable) { _shadow->setVisible(enable); }
            
            bool isEnableAnimation() const { return _enableAnimation; }
            void enableAnimation(bool enable) { _enableAnimation = enable; }
            
            bool initWithFile(const std::string& fileName, cocos2d::ui::Widget::TextureResType resType);
            virtual void clear() override;
            virtual void update(float dt) override;
            virtual bool handleMessage(const Telegram& msg) override { return false; }
            
        private:
            
            GameObject2* _owner;
            
            cocos2d::Sprite* _image;
            cocos2d::Sprite* _shadow;
            
            bool _enableAnimation;
            
            std::map<int, std::vector<std::string>> _animations;
            float _animationSwapTime;
            float _accumulatedTime;
            
        };
        
    }
}









