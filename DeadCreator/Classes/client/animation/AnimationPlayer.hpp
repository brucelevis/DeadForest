//
//  AnimationPlayer.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 14..
//
//

#pragma once

#include <deque>
#include <string>

#include "EntityType.hpp"
#include "cocos2d.h"

namespace realtrick
{
    namespace client
    {
        
        class AnimationBase;
        class DynamicEntity;
        
        class AnimationPlayer
        {
            
        public:
            
            AnimationPlayer(DynamicEntity* owner, AnimationBase* animation, int zOrder);
            
            void pushAnimationFrames(AnimationBase* animation);
            void pushOneFrame(const std::pair<std::string, int>& pair_string_int);
            void pushOneFrameUnique(AnimationBase* anim, int index);
            void pushFramesAtoB(AnimationBase* anim,int startIndex, int endIndex);
            
            void processAnimation(float dt);
            
            bool isQueueEmpty() const { return _frameQueue.empty(); }
            void clearFrameQueue() { _frameQueue.clear(); }
            
            AnimationBase* getCurrAnimation() const { return _currAnimation; }
            void setCurrAnimation(AnimationBase* animation) { _currAnimation = animation; }
            
            int getFrameIndex() const;
            int getQueueSize() const { return static_cast<int>(_frameQueue.size()); }
            void setVisible(bool enable);
            
            void enableShadow(bool enable);
            void enableNormal(bool enable);
            
            void enableForceStop(bool enable) { _isForceStopped = enable; }
            bool isForceStopped() const { return _isForceStopped; }
            
            std::string getCurrFrameName() const { return _currFrameName; }
            
        private:
            
            std::string _getFrameName() const;
            
        private:
            
            DynamicEntity*                                          _owner;
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
}









