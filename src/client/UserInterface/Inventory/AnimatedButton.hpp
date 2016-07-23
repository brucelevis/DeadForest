//
//  AnimatedButton.hpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 2. 21..
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace realtrick
{
    namespace userinterface
    {
        
        class AnimatedButton : public cocos2d::Node
        {
            
        public:
            
            enum class ActionCommand { SHOW, HIDE };
            
        public:
            
            static AnimatedButton* create(const char* normal, const char* selected, cocos2d::ui::Widget::TextureResType type = cocos2d::ui::Widget::TextureResType::LOCAL);
            
            void setAnimationFrameNames(const std::vector<std::string>& frameNames) { _animationFrameNames = frameNames; }
            
            const std::vector<std::string>& getAnimationFrameNames() const { return _animationFrameNames; }
            
            void setAnimationInterval(float interval) { _animInterval = interval; }
            
            void setButtonCallback(const std::function<void(cocos2d::Ref*, cocos2d::ui::Widget::TouchEventType type)>& callback) { _button->addTouchEventListener(callback); }
            
            float getAnimationInterval() const { return _animInterval; }
            
            void setButtonSize(cocos2d::Size size);
            
            cocos2d::Size getButtonSize() const { return _buttonSize; }
            
            void show();
            
            void hide();
            
            bool isActive() const { return (_button->getOpacity() == 255); }
            
            bool isShowing() const { return _isShowing; }
            
            bool isHidding() const { return _isHidding; }
            
            void setTopInfoString(const std::string info) { _topInfo->setString(info); }
            
            void setBottomInfoString(const std::string info) { _bottomInfo->setString(info); }
            
        private:
            
            std::vector<std::string>                                                        _animationFrameNames;
            
            cocos2d::ui::Button*                                                            _button;
            
            cocos2d::Sprite*                                                                _sprite;
            
            cocos2d::ui::Text*                                                              _topInfo;
            
            cocos2d::ui::Text*                                                              _bottomInfo;
            
            int                                                                             _currFrame;
            
            float                                                                           _animInterval;
            
            std::list<ActionCommand>                                                        _commandQueue;
            
            cocos2d::ui::Widget::TextureResType                                             _texType;

            cocos2d::Size                                                                   _buttonSize;
            
            bool                                                                            _isShowing;
            
            bool                                                                            _isHidding;
            
        private:
            
            AnimatedButton();
            
            virtual ~AnimatedButton();
            
            bool init(const char* normal, const char* selected, cocos2d::ui::Widget::TextureResType type);
            
            void _showOne();
            
            void _hideOne();
            
            void _setFrame(int idx);
    
        };
        
    }
}









