#pragma once

#include <deque>
#include <string>

#include "EntityType.hpp"

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
                
        void enableForceStop(bool enable) { _isForceStopped = enable; }
        
        bool isForceStopped() const { return _isForceStopped; }
        
        std::string getCurrFrameName() const            { return _currFrameName; }
        
    private:
        
        AnimationPlayer() = delete;
        
        std::string _getFrameName() const;
        
    private:
        
        EntityHuman*                                    _owner;
        
        AnimationBase*                                  _currAnimation;
        
        std::deque<std::pair<std::string, int>>         _frameQueue;
        
        float                                           _accumulatedTime;
        
        std::string                                     _currFrameName;
        
        bool                                            _isForceStopped;
        
    };
    
}


















