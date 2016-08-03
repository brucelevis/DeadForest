//
//  HumanFistAnimations.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 14..
//
//

#pragma once

#include "AnimationBase.hpp"
#include "Singleton.hpp"

namespace realtrick
{
    namespace client
    {
        
        class AnimHumanFistIdleLoop : public AnimationBase, public Singleton<AnimHumanFistIdleLoop>
        {
            
        private:
            
            friend class Singleton<AnimHumanFistIdleLoop>;
            virtual ~AnimHumanFistIdleLoop() = default;
            
            AnimHumanFistIdleLoop()
            {
                _startFrame     = 0;
                _maxFrame       = 21;
                _fileName       = "HumanFistIdleLoop";
                _priority       = 0;
                _frameSwapTime  = 0.1f;
                _isReversePlay  = false;
            }
            
        };
        
        
        class AnimHumanFistOut : public AnimationBase, public Singleton<AnimHumanFistOut>
        {
            
        private:
            
            friend class Singleton<AnimHumanFistOut>;
            virtual ~AnimHumanFistOut() = default;
            
            AnimHumanFistOut()
            {
                _startFrame     = 0;
                _maxFrame       = 5;
                _fileName       = "HumanFistIn";
                _priority       = 0;
                _frameSwapTime  = 0.1f;
                _isReversePlay  = true;
            }
            
        };
        
        
        class AnimHumanFistIn : public AnimationBase, public Singleton<AnimHumanFistIn>
        {
            
        private:
            
            friend class Singleton<AnimHumanFistIn>;
            virtual ~AnimHumanFistIn() = default;
            
            AnimHumanFistIn()
            {
                _startFrame     = 0;
                _maxFrame       = 5;
                _fileName       = "HumanFistIn";
                _priority       = 0;
                _frameSwapTime  = 0.1f;
                _isReversePlay  = false;
            }
            
        };
        
        
        
        class AnimHumanFistMoveLoop : public AnimationBase, public Singleton<AnimHumanFistMoveLoop>
        {
            
        private:
            
            friend class Singleton<AnimHumanFistMoveLoop>;
            virtual ~AnimHumanFistMoveLoop() = default;
            
            AnimHumanFistMoveLoop()
            {
                _startFrame     = 0;
                _maxFrame       = 12;
                _fileName       = "HumanFistMoveLoop";
                _priority       = 4;
                _frameSwapTime  = 0.05f;
                _isReversePlay  = false;
            }
            
        };
        
        
        class AnimHumanFistAttackReady : public AnimationBase, public Singleton<AnimHumanFistAttackReady>
        {
            
        private:
            
            friend class Singleton<AnimHumanFistAttackReady>;
            virtual ~AnimHumanFistAttackReady() = default;
            
            AnimHumanFistAttackReady()
            {
                _startFrame     = 0;
                _maxFrame       = 5;
                _fileName       = "HumanFistAttackReady";
                _priority       = 3;
                _frameSwapTime  = 0.07f;
                _isReversePlay  = false;
            }
            
        };
        
        
        class AnimHumanFistAttackRelease : public AnimationBase, public Singleton<AnimHumanFistAttackRelease>
        {
            
        private:
            
            friend class Singleton<AnimHumanFistAttackRelease>;
            virtual ~AnimHumanFistAttackRelease() = default;
            
            AnimHumanFistAttackRelease()
            {
                _startFrame     = 0;
                _maxFrame       = 5;
                _fileName       = "HumanFistAttackReady";
                _priority       = 3;
                _frameSwapTime  = 0.07f;
                _isReversePlay  = true;
            }
            
        };
        
        
        class AnimHumanFistAttackHover : public AnimationBase, public Singleton<AnimHumanFistAttackHover>
        {
            
        private:
            
            friend class Singleton<AnimHumanFistAttackHover>;
            virtual ~AnimHumanFistAttackHover() = default;
            
            AnimHumanFistAttackHover()
            {
                _startFrame     = 0;
                _maxFrame       = 2;
                _fileName       = "HumanFistAttackHover";
                _priority       = 3;
                _frameSwapTime  = 0.07f;
                _isReversePlay  = false;
            }
            
        };
        
        
        class AnimHumanFistAttackAction : public AnimationBase, public Singleton<AnimHumanFistAttackAction>
        {
            
        private:
            
            friend class Singleton<AnimHumanFistAttackAction>;
            virtual ~AnimHumanFistAttackAction() = default;
            
            AnimHumanFistAttackAction()
            {
                _startFrame     = 0;
                _maxFrame       = 2;
                _fileName       = "HumanFistAttackAction";
                _priority       = 5;
                _frameSwapTime  = 0.07f;
                _isReversePlay  = false;
            }
            
        };
        
    }
}









