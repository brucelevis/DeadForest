//
//  RenderableComponent.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 31..
//
//

#include "RenderableComponent.hpp"
#include "GameObject2.hpp"
using namespace cocos2d;
using namespace realtrick::client;


bool RenderableComponent::initWithFile(const std::string& fileName, cocos2d::ui::Widget::TextureResType resType)
{
    if ( !Node::init() )
        return false;
    
    _shadow = Sprite::create();
    _shadow->setPosition(10, -5);
    addChild(_image);
    
    _image = Sprite::create();
    addChild(_image);
    
    if ( resType == ui::Widget::TextureResType::LOCAL )
    {
        _image->setTexture(fileName);
        _shadow->setTexture(fileName);
    }
    else // PLIST
    {
        _image->setSpriteFrame(fileName);
        _shadow->setSpriteFrame(fileName);
    }
    
    _shadow->setColor(Color3B::BLACK);
    
    return true;
}


void RenderableComponent::clear()
{
    _owner = nullptr;
    this->removeFromParentAndCleanup(true);
    _animations.clear();
}


void RenderableComponent::update(float dt)
{
    _image->setRotation(_owner->getRotation());
    _shadow->setRotation(_owner->getRotation());
    
    if ( _enableAnimation )
    {
        _accumulatedTime += dt;
        if ( _animationSwapTime > _accumulatedTime )
        {
            _accumulatedTime = 0.0f;
            
            
        }
    }
}









