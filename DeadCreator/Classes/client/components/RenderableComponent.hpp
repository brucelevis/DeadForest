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
        class AnimationBase;
        
        class RenderableComponent : public ComponentBase
        {
            
        public:
            
            explicit RenderableComponent(GameObject2* owner);
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
            
            bool initWithFile(const std::string& fileName, cocos2d::ui::Widget::TextureResType resType);
            
            bool isEnableShadow() const { return _shadow->isVisible(); }
            void enableShadow(bool enable) { _shadow->setVisible(enable); }
            
            // usual suspect
            void setAnimation(AnimationBase* animation);
            
            virtual void clear() override;
            virtual void update(float dt) override;
            virtual bool handleMessage(const Telegram& msg) override { return false; }
            
        private:
            
            void setImageFrame(const std::string& fileName);
            
        private:
            
            GameObject2* _owner;
            
            cocos2d::ui::Widget::TextureResType _texResType;
            
            cocos2d::Node* _root;
            cocos2d::Sprite* _image;
            cocos2d::Sprite* _shadow;
            
            std::deque<std::string> _frameQueue;
            AnimationBase* _currentAnimation;
            float _accumulatedTime;
            
        };
        
    }
}









