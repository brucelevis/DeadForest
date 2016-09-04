//
//  Animator.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 14..
//
//

#pragma once

#include <deque>
#include <string>

#include "Types.hpp"
#include "cocos2d.h"

namespace realtrick
{
    namespace client
    {
        
        class AnimationBase;
        class HumanBase;
        
        class Animator
        {
            
        public:
            
            explicit Animator(HumanBase* owner);
            
            void pushAnimationFrames(AnimationBase* animation);
            void pushFramesAtoB(AnimationBase* anim,int startIndex, int endIndex);
            
            void processAnimation(float dt);
            
            bool isQueueEmpty() const { return _frameQueue.empty(); }
            void clearFrameQueue() { _frameQueue.clear(); }
            
            AnimationBase* getCurrAnimation() const { return _currAnimation; }
            
            int getFrameIndex() const;
            void setVisible(bool enable);
            
            void setRotation(float rotation);
            void setShadowVisible(bool visible) { _shadowSprite->setVisible(visible); }
            
            void enableNormalMap();
            void disableNormalMap();
            
        private:
            
            std::string getFrameName() const;
            
        private:
            
            HumanBase*                                              _owner;
            AnimationBase*                                          _currAnimation;
            
            std::deque<std::pair<std::string, int>>                 _frameQueue;
            float                                                   _accumulatedTime;
            
            cocos2d::Sprite*                                        _baseSprite;
            cocos2d::Sprite*                                        _shadowSprite;
            cocos2d::Sprite*                                        _normalSprite;
            
            std::string                                             _currFrameName;
            int                                                     _currFrame;
            
        };
        
    }
}









