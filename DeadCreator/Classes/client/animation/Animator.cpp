//
//  Animator.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 14..
//
//

#include <algorithm>

#include "Animator.hpp"
#include "AnimationBase.hpp"
#include "HumanBase.hpp"
#include "HumanOwnedStates.hpp"
#include "Game.hpp"
using namespace cocos2d;
using namespace realtrick::client;


Animator::Animator(Node* owner) :
_owner(owner),
_currAnimation(nullptr),
_accumulatedTime(0.0f),
_baseSprite(nullptr),
_shadowSprite(nullptr),
_currFrameName("##invalid"),
_currFrame(0),
_isForceStopped(false)
{
    _baseSprite = Sprite::create();
    _owner->addChild(_baseSprite);
    
    _shadowSprite = Sprite::create();
    _shadowSprite->setPosition(10.0f, -7.0f);
    _owner->addChild(_shadowSprite);
    
    _baseSprite->setLocalZOrder(1);
    _shadowSprite->setLocalZOrder(0);
}


void Animator::pushAnimationFrames(AnimationBase* animation)
{
    if( _currAnimation && _currAnimation->getPriority() <= animation->getPriority())
    {
        _frameQueue.clear();
    }
    _currAnimation = animation;
    
    int startIndex, endIndex;
    if(animation->isReversePlay() == true)
    {
        startIndex = animation->getMaxFrame() - 1;
        endIndex = animation->getStartFrame();
        for(int i = startIndex ; i >= endIndex; -- i)
        {
            std::string frameName = animation->getFileName() + _to_string(i);
            _frameQueue.push_back(std::make_pair(frameName, i));
        }
    }
    else
    {
        startIndex = animation->getStartFrame();
        endIndex = animation->getMaxFrame();
        for(int i = startIndex ; i < endIndex; ++ i)
        {
            std::string frameName = animation->getFileName() + _to_string(i);
            _frameQueue.push_back(std::make_pair(frameName, i));
        }
    }
    
}


void Animator::pushFramesAtoB(AnimationBase* anim, int startIndex, int endIndex)
{
    if( _currAnimation && _currAnimation->getPriority() <= anim->getPriority())
    {
        _frameQueue.clear();
    }
    _currAnimation = anim;
    
    if( startIndex < endIndex )
    {
        if( startIndex < 0 || endIndex >= anim->getMaxFrame())
            return ;
        
        for(int i = startIndex ; i <= endIndex ; ++ i)
        {
            std::string frameName = anim->getFileName() + _to_string(i);
            _frameQueue.push_back(std::make_pair(frameName, i));
        }
    }
    else
    {
        if( endIndex < 0 || startIndex >= anim->getMaxFrame() )
            return ;
        
        for(int i = startIndex ; i >= endIndex ; -- i)
        {
            std::string frameName = anim->getFileName() + _to_string(i);
            _frameQueue.push_back(std::make_pair(frameName, i));
        }
    }
}


void Animator::pushOneFrameUnique(AnimationBase* anim, int index)
{
    // 인덱스와 프레임이름이 하나라도 틀리면 푸시한다.
    if ( _frameQueue.empty() )
    {
        _frameQueue.push_back({ anim->getFileName() + _to_string(index), index });
        _currAnimation = anim;
    }
    else if ( _frameQueue.back().second != index || _frameQueue.back().first != anim->getFileName() + _to_string(index) )
    {
        _frameQueue.push_back({ anim->getFileName() + _to_string(index), index });
        _currAnimation = anim;
    }
}


void Animator::processAnimation(float dt)
{
    if( _frameQueue.empty() )
        return ;
    
    _accumulatedTime += dt;
    if( _currAnimation->getFrameSwapTime() <= _accumulatedTime && !_isForceStopped )
    {
        _accumulatedTime = 0.0f;
        
        _currFrameName = getFrameName();
        _currFrame = getFrameIndex();
        _frameQueue.pop_front();
        
        _baseSprite->setSpriteFrame(_currFrameName + ".png");
        _shadowSprite->setSpriteFrame(_currFrameName + ".png");
        _shadowSprite->setColor(Color3B::BLACK);
    }
}


void Animator::setRotation(float rotation)
{
    _baseSprite->setRotation(rotation);
    _shadowSprite->setRotation(rotation);
}


std::string Animator::getFrameName() const
{
    return (_frameQueue.empty() ? "no animation frame" : _frameQueue.front().first);
}


int Animator::getFrameIndex() const
{
    return (_frameQueue.empty() ? -1 : _frameQueue.front().second);
}


void Animator::setVisible(bool enable)
{
    _baseSprite->setVisible(enable);
}









