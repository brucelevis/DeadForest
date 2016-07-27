//
//  AnimationPlayer.cpp
//  TheDeadForest
//
//  Created by 남준현 on 2015. 11. 14..
//
//

#include <algorithm>

#include "AnimationPlayer.hpp"
#include "AnimationBase.hpp"
#include "EntityHuman.hpp"
#include "HumanOwnedStates.hpp"
#include "GameManager.hpp"
using namespace cocos2d;
using namespace realtrick::client;


AnimationPlayer::AnimationPlayer(EntityHuman* owner, AnimationBase* animation, int zOrder) :
_owner(owner),
_accumulatedTime(0.0f),
_currFrame(0),
_isForceStopped(false)
{
    _currAnimation = animation;
    
    _baseSprite = Sprite::create();
    _owner->addChild(_baseSprite);
    
    _shadowSprite = Sprite::create();
    _shadowSprite->setPosition(10.0f, -7.0f);
    _owner->addChild(_shadowSprite);
    
    _normalSprite = Sprite::create();
    _normalSprite->setVisible(false);
    _owner->addChild(_normalSprite);
    
    _baseSprite->setLocalZOrder(zOrder);
    _shadowSprite->setLocalZOrder(zOrder - 1);
    _normalSprite->setLocalZOrder(zOrder + 1);
}


void AnimationPlayer::pushAnimationFrames(AnimationBase* animation)
{
    if(_currAnimation->getPriority() <= animation->getPriority())
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


void AnimationPlayer::pushFramesAtoB(AnimationBase* anim, int startIndex, int endIndex)
{
    if(_currAnimation->getPriority() <= anim->getPriority())
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


void AnimationPlayer::pushOneFrameUnique(AnimationBase* anim, int index)
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


void AnimationPlayer::processAnimation(float dt)
{
    _baseSprite->setRotation(_owner->getBodyRot());
    _shadowSprite->setRotation(_owner->getBodyRot());
    _normalSprite->setRotation(_owner->getBodyRot());
    
    if( _frameQueue.empty() == true )
        return ;
    
    _accumulatedTime += dt;
    if( _currAnimation->getFrameSwapTime() <= _accumulatedTime && !_isForceStopped )
    {
        _accumulatedTime = 0.0f;
        
        _currFrameName = _getFrameName();
        _currFrame = getFrameIndex();
        _frameQueue.pop_front();
        
        _baseSprite->setSpriteFrame(_currFrameName + ".png");
        
        _shadowSprite->setSpriteFrame(_currFrameName + ".png");
        _shadowSprite->setColor(Color3B::BLACK);
        
        _normalSprite->setSpriteFrame(_currFrameName + "_n.png");
    }
}


std::string AnimationPlayer::_getFrameName() const
{
    return (_frameQueue.empty() ? "no animation frame" : _frameQueue.front().first);
}


int AnimationPlayer::getFrameIndex() const
{
    return (_frameQueue.empty() ? -1 : _frameQueue.front().second);
}


void AnimationPlayer::pushOneFrame(const std::pair<std::string, int>& pair_string_int)
{
    _frameQueue.push_back(pair_string_int);
}


void AnimationPlayer::setVisible(bool enable)
{
    _baseSprite->setVisible(enable);
}


void AnimationPlayer::enableShadow(bool enable)
{
    _shadowSprite->setVisible(enable);
}


void AnimationPlayer::enableNormal(bool enable)
{
    _normalSprite->setVisible(enable);
}









