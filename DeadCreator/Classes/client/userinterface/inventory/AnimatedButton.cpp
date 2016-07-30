//
//  AnimatedButton.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 2. 21..
//
//

#include "AnimatedButton.hpp"
using namespace realtrick::client;
using namespace cocos2d;

AnimatedButton::AnimatedButton() :
_button(nullptr),
_sprite(nullptr),
_currFrame(0),
_animInterval(0.0f),
_texType(ui::Widget::TextureResType::LOCAL),
_buttonSize(Size::ZERO),
_isShowing(false),
_isHidding(false)
{}


AnimatedButton::~AnimatedButton()
{
}


AnimatedButton* AnimatedButton::create(const char* normal, const char* selected, cocos2d::ui::Widget::TextureResType type)
{
    AnimatedButton* ret = new (std::nothrow) AnimatedButton();
    if ( ret && ret->init(normal, selected, type) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool AnimatedButton::init(const char* normal, const char* selected, cocos2d::ui::Widget::TextureResType type)
{
    if ( !Node::init() )
        return false;
    
    _button = ui::Button::create(normal, selected, "", type);
    _button->setTouchEnabled(false);
    _button->setOpacity(0);
    addChild(_button, 1);
    
    _sprite = Sprite::create();
    addChild(_sprite, 2);
    
    _topInfo = ui::Text::create("", "SpecialElite.TTF", 12);
    _topInfo->setPosition(Vec2(_button->getContentSize().width / 2.0f, _button->getContentSize().height - 20.0f));
    _topInfo->setOpacity(0);
    _button->addChild(_topInfo);
    
    _bottomInfo = ui::Text::create("", "SpecialElite.TTF", 12);
    _bottomInfo->setPosition(Vec2(_button->getContentSize().width / 2.0f, 10.0f));
    _bottomInfo->setOpacity(0);
    _button->addChild(_bottomInfo);
    
    _texType = type;
    
    return true;
}


void AnimatedButton::show()
{
    if ( _commandQueue.empty() && !isActive() )
    {
        _commandQueue.push_back(ActionCommand::SHOW);
        _showOne();
    }
    
    else if ( !_commandQueue.empty() )
    {
        if ( _commandQueue.back() == ActionCommand::HIDE )
        {
            _commandQueue.push_back(ActionCommand::SHOW);
        }
    }
}


void AnimatedButton::hide()
{
    if ( _commandQueue.empty() && isActive() )
    {
        _commandQueue.push_back(ActionCommand::HIDE);
        _hideOne();
    }
    
    else if ( !_commandQueue.empty() )
    {
        if ( _commandQueue.back() == ActionCommand::SHOW )
        {
            _commandQueue.push_back(ActionCommand::HIDE);
        }
    }
}


void AnimatedButton::_showOne()
{
    _currFrame = 0;
    _sprite->runAction(Sequence::create(CallFunc::create([&]
                                                         {
                                                             _sprite->setVisible(true);
                                                             _button->setOpacity(0);
                                                             _isShowing = true;
                                                             _topInfo->runAction(FadeTo::create((int)_animationFrameNames.size() * _animInterval, 255));
                                                             _bottomInfo->runAction(FadeTo::create((int)_animationFrameNames.size() * _animInterval, 255));
                                                         }),
                                        Repeat::create(Sequence::create(CallFunc::create([&]{ _setFrame(_currFrame); }),
                                                                        DelayTime::create(_animInterval),
                                                                        CallFunc::create([&]{ _currFrame ++; }),
                                                                        nullptr), (int)_animationFrameNames.size()),
                                        CallFunc::create([&]
                                                         {
                                                             _isShowing = false;
                                                             _sprite->setVisible(false);
                                                             _button->setOpacity(255);
                                                             _button->setTouchEnabled(true);
                                                             _commandQueue.pop_front();
                                                             if ( !_commandQueue.empty() )
                                                             {
                                                                 _hideOne();
                                                             }
                                                         }),
                                        nullptr));
}


void AnimatedButton::_hideOne()
{
    _currFrame = (int)_animationFrameNames.size() - 1;
    _sprite->runAction(Sequence::create(CallFunc::create([&]
                                                         {
                                                             _sprite->setVisible(true);
                                                             _button->setOpacity(0);
                                                             _button->setTouchEnabled(false);
                                                             _isHidding = true;
                                                             _topInfo->runAction(FadeTo::create((int)_animationFrameNames.size() * _animInterval, 0));
                                                             _bottomInfo->runAction(FadeTo::create((int)_animationFrameNames.size() * _animInterval, 0));
                                                         }),
                                        Repeat::create(Sequence::create(CallFunc::create([&]{ _setFrame(_currFrame); }),
                                                                        DelayTime::create(_animInterval),
                                                                        CallFunc::create([&]{ _currFrame --; }),
                                                                        nullptr), (int)_animationFrameNames.size()),
                                        CallFunc::create([&]
                                                         {
                                                             _isHidding = false;
                                                             _sprite->setVisible(false);
                                                             _commandQueue.pop_front();
                                                             if ( !_commandQueue.empty() )
                                                             {
                                                                 _showOne();
                                                             }
                                                         }),
                                        nullptr));
}


void AnimatedButton::_setFrame(int idx)
{
    if ( _texType == ui::Widget::TextureResType::LOCAL )
    {
        _sprite->setTexture(_animationFrameNames[idx]);
    }
    else
    {
        SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(_animationFrameNames.at(_currFrame));
        _sprite->setSpriteFrame(spriteFrame);
    }
}


void AnimatedButton::setButtonSize(cocos2d::Size size)
{
    float sx, sy;
    sx = size.width / _button->getContentSize().width;
    sy = size.height / _button->getContentSize().height;
    
    _buttonSize = size;
    _button->setScale(sx, sy);
    _sprite->setScale(sx, sy);
}









