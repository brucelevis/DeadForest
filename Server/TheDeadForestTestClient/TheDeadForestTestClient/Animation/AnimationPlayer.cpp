
#include "AnimationPlayer.hpp"
#include "AnimationBase.hpp"
#include "EntityHuman.hpp"
#include "HumanOwnedStates.hpp"
#include "GameManager.hpp"


namespace realtrick
{
    
    AnimationPlayer::AnimationPlayer(EntityHuman* owner, AnimationBase* animation, int zOrder) :
    _owner(owner),
    _accumulatedTime(0.0f),
    _isForceStopped(false)
    {
        _currAnimation = animation;
    }
    
    
    void AnimationPlayer::pushAnimationFrames(AnimationBase* animation)
    {
        if(_currAnimation->getPriority() <= animation->getPriority())
        {
            _frameQueue.clear();
        }
        
        int startIndex, endIndex;
        if(animation->isReversePlay() == true)
        {
            startIndex = animation->getMaxFrame() - 1;
            endIndex = animation->getStartFrame();
            for(int i = startIndex ; i >= endIndex; -- i)
            {
                std::string frameName = animation->getFileName() + GameManager::_to_string(i);
                _frameQueue.push_back(std::make_pair(frameName, i));
            }
        }
        else
        {
            startIndex = animation->getStartFrame();
            endIndex = animation->getMaxFrame();
            for(int i = startIndex ; i < endIndex; ++ i)
            {
                std::string frameName = animation->getFileName() + GameManager::_to_string(i);
                _frameQueue.push_back(std::make_pair(frameName, i));
            }
        }
        
        _currAnimation = animation;
    }
    
    
    void AnimationPlayer::pushFramesAtoB(AnimationBase* anim, int startIndex, int endIndex)
    {
        if(_currAnimation->getPriority() <= anim->getPriority())
        {
            _frameQueue.clear();
        }
        
        if( startIndex < endIndex )
        {
            if( startIndex < 0 || endIndex >= anim->getMaxFrame())
                return ;
            
            for(int i = startIndex ; i <= endIndex ; ++ i)
            {
                std::string frameName = anim->getFileName() + GameManager::_to_string(i);
                _frameQueue.push_back(std::make_pair(frameName, i));
            }
        }
        else
        {
            if( endIndex < 0 || startIndex >= anim->getMaxFrame() )
                return ;
            
            for(int i = startIndex ; i >= endIndex ; -- i)
            {
                std::string frameName = anim->getFileName() + GameManager::_to_string(i);
                _frameQueue.push_back(std::make_pair(frameName, i));
            }
        }
        
        _currAnimation = anim;
    }
    
    
    void AnimationPlayer::pushOneFrameUnique(AnimationBase* anim, int index)
    {
        // 인덱스와 프레임이름이 하나라도 틀리면 푸시한다.
        if ( _frameQueue.empty() )
        {
            _frameQueue.push_back({ anim->getFileName() + GameManager::_to_string(index), index });
            _currAnimation = anim;
        }
        else if ( _frameQueue.back().second != index || _frameQueue.back().first != anim->getFileName() + GameManager::_to_string(index) )
        {
            _frameQueue.push_back({ anim->getFileName() + GameManager::_to_string(index), index });
            _currAnimation = anim;
        }
    }
    
    
    void AnimationPlayer::processAnimation(float dt)
    {
        if( _frameQueue.empty() == true )
            return ;
        
        _accumulatedTime += dt;
        if( _currAnimation->getFrameSwapTime() <= _accumulatedTime )
        {
            if ( !_isForceStopped )
            {
                _accumulatedTime = 0.0f;
                
                _currFrameName = _getFrameName();
                _frameQueue.pop_front();
                
            }
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
    
}












