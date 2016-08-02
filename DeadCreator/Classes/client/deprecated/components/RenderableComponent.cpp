//
//  RenderableComponent.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 31..
//
//

#include "RenderableComponent.hpp"
#include "GameObject2.hpp"
#include "Animations.hpp"
#include "StringHelper.hpp"
using namespace cocos2d;
using namespace realtrick::client;

RenderableComponent::RenderableComponent(GameObject2* owner) :
_owner(owner),
_texResType(cocos2d::ui::Widget::TextureResType::LOCAL),
_root(nullptr),
_image(nullptr),
_shadow(nullptr),
_currentAnimation(nullptr),
_accumulatedTime(0.0f)
{
    _type = ComponentType::RENDERABLE;
}

bool RenderableComponent::initWithFile(const std::string& fileName, cocos2d::ui::Widget::TextureResType resType)
{
    _texResType = resType;
    
    _root = Node::create();
    
    _shadow = Sprite::create();
    _shadow->setPosition(10, -5);
    _root->addChild(_image);
    
    _image = Sprite::create();
    _root->addChild(_image);
    
    setImageFrame(fileName);
    
    return true;
}


void RenderableComponent::setImageFrame(const std::string& fileName)
{
    if ( _texResType == ui::Widget::TextureResType::LOCAL )
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
}


void RenderableComponent::clear()
{
    _owner = nullptr;
    _root->removeFromParentAndCleanup(true);
    _frameQueue.clear();
    _currentAnimation = nullptr;
}


void RenderableComponent::update(float dt)
{
    _image->setRotation(_owner->getRotation());
    _shadow->setRotation(_owner->getRotation());
    
    if ( !_frameQueue.empty() )
    {
        _accumulatedTime += dt;
        if( _currentAnimation->getFrameSwapTime() <= _accumulatedTime )
        {
            _accumulatedTime = 0.0f;
        
            setImageFrame(_frameQueue.front() + ".png");
            _frameQueue.pop_front();
        }
    }
}


void RenderableComponent::setAnimation(AnimationBase* animation)
{
    _frameQueue.clear();
    _currentAnimation = animation;
    
    if( !animation->isReversePlay() )
    {
        for(int i = animation->getStartFrame() ; i < animation->getMaxFrame(); ++ i)
            _frameQueue.push_back(animation->getFileName() + _to_string(i));
    }
    
    else
    {
        for(int i = animation->getMaxFrame() - 1 ; i >= animation->getStartFrame(); -- i)
            _frameQueue.push_back(animation->getFileName() + _to_string(i));
    }
}









