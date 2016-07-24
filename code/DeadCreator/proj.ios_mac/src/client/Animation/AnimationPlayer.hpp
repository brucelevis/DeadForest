//
//  AnimationPlayer.hpp
//  TheDeadForest
//
//  Created by 남준현 on 2015. 11. 14..
//  
//

#pragma once

#include <deque>
#include <string>

#include "EntityType.hpp"
#include "cocos2d.h"

namespace realtrick
{
    
    class AnimationBase;
    class EntityHuman;
    
    class AnimationPlayer
    {
    
    public:
        
        AnimationPlayer(EntityHuman* owner, AnimationBase* animation, int zOrder);
        
        void pushAnimationFrames(AnimationBase* animation);
        
        void pushOneFrame(const std::pair<std::string, int>& pair_string_int);
        
        void pushOneFrameUnique(AnimationBase* anim, int index);
        
        void pushFramesAtoB(AnimationBase* anim,int startIndex, int endIndex);
        
        void processAnimation(float dt);
        
        bool isQueueEmpty() const                       { return _frameQueue.empty(); }
        
        void clearFrameQueue()                          { _frameQueue.clear(); }
        
        AnimationBase* getCurrAnimation() const         { return _currAnimation; }
        
        void setCurrAnimation(AnimationBase* animation) { _currAnimation = animation; }
        
        int getFrameIndex() const;
        
        int getQueueSize() const { return static_cast<int>(_frameQueue.size()); }
        
        void setVisible(bool enable);
        
        void enableShadow(bool enable);
        
        void enableNormal(bool enable);
        
        void enableForceStop(bool enable) { _isForceStopped = enable; }
        
        bool isForceStopped() const { return _isForceStopped; }
        
        std::string getCurrFrameName() const            { return _currFrameName; }
        
        cocos2d::Sprite* getBaseSprite() const          { return _baseSprite; }
        
        cocos2d::Sprite* getShadowSprite() const        { return _shadowSprite; }
        
    private:
        
        AnimationPlayer() = delete;
        
        std::string _getFrameName() const;
        
    private:
        
        EntityHuman*                                            _owner;
        
        AnimationBase*                                          _currAnimation;
        
        std::deque<std::pair<std::string, int>>                 _frameQueue;
        
        float                                                   _accumulatedTime;
        
        cocos2d::Sprite*                                        _baseSprite;
        
        cocos2d::Sprite*                                        _shadowSprite;
        
        cocos2d::Sprite*                                        _normalSprite;
        
        std::string                                             _currFrameName;
        
        int                                                     _currFrame;
        
        bool                                                    _isForceStopped;
        
    };
    
}


















