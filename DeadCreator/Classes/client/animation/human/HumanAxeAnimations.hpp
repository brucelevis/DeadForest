//
//  HumanAxeAnimations.hpp
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
        
        class AnimHumanAxeIdleLoop : public AnimationBase, public Singleton<AnimHumanAxeIdleLoop>
        {
            
        private:
            
            friend class Singleton<AnimHumanAxeIdleLoop>;
            virtual ~AnimHumanAxeIdleLoop() = default;
            
            AnimHumanAxeIdleLoop()
            {
                _startFrame     = 0;
                _maxFrame       = 20;
                _fileName       = "HumanAxeIdleLoop";
                _priority       = 0;
                _frameSwapTime  = 0.1f;
                _isReversePlay  = false;
            }
            
        };
        
        
        class AnimHumanAxeOut : public AnimationBase, public Singleton<AnimHumanAxeOut>
        {
            
        private:
            
            friend class Singleton<AnimHumanAxeOut>;
            virtual ~AnimHumanAxeOut() = default;
            
            AnimHumanAxeOut()
            {
                _startFrame     = 0;
                _maxFrame       = 4;
                _fileName       = "HumanAxeIn";
                _priority       = 4;
                _frameSwapTime  = 0.1f;
                _isReversePlay  = true;
            }
            
        };
        
        
        class AnimHumanAxeIn : public AnimationBase, public Singleton<AnimHumanAxeIn>
        {
            
        private:
            
            friend class Singleton<AnimHumanAxeIn>;
            virtual ~AnimHumanAxeIn() = default;
            
            AnimHumanAxeIn()
            {
                _startFrame     = 0;
                _maxFrame       = 4;
                _fileName       = "HumanAxeIn";
                _priority       = 4;
                _frameSwapTime  = 0.1f;
                _isReversePlay  = false;
            }
            
        };
        
        
        
        class AnimHumanAxeMoveLoop : public AnimationBase, public Singleton<AnimHumanAxeMoveLoop>
        {
            
        private:
            
            friend class Singleton<AnimHumanAxeMoveLoop>;
            virtual ~AnimHumanAxeMoveLoop() = default;
            
            AnimHumanAxeMoveLoop()
            {
                _startFrame     = 0;
                _maxFrame       = 16;
                _fileName       = "HumanAxeMoveLoop";
                _priority       = 4;
                _frameSwapTime  = 0.05f;
                _isReversePlay  = false;
            }
            
        };
        
        
        class AnimHumanAxeAttackReady : public AnimationBase, public Singleton<AnimHumanAxeAttackReady>
        {
            
        private:
            
            friend class Singleton<AnimHumanAxeAttackReady>;
            virtual ~AnimHumanAxeAttackReady() = default;
            
            AnimHumanAxeAttackReady()
            {
                _startFrame     = 0;
                _maxFrame       = 7;
                _fileName       = "HumanAxeAttackReady";
                _priority       = 3;
                _frameSwapTime  = 0.07f;
                _isReversePlay  = false;
            }
            
        };
        
        
        class AnimHumanAxeAttackRelease : public AnimationBase, public Singleton<AnimHumanAxeAttackRelease>
        {
            
        private:
            
            friend class Singleton<AnimHumanAxeAttackRelease>;
            virtual ~AnimHumanAxeAttackRelease() = default;
            
            AnimHumanAxeAttackRelease()
            {
                _startFrame     = 0;
                _maxFrame       = 7;
                _fileName       = "HumanAxeAttackReady";
                _priority       = 3;
                _frameSwapTime  = 0.07f;
                _isReversePlay  = true;
            }
            
        };

        
        
        class AnimHumanAxeAttackHover : public AnimationBase, public Singleton<AnimHumanAxeAttackHover>
        {
            
        private:
            
            friend class Singleton<AnimHumanAxeAttackHover>;
            virtual ~AnimHumanAxeAttackHover() = default;
            
            AnimHumanAxeAttackHover()
            {
                _startFrame     = 0;
                _maxFrame       = 3;
                _fileName       = "HumanAxeAttackHover";
                _priority       = 3;
                _frameSwapTime  = 0.07f;
                _isReversePlay  = false;
            }
            
        };
      
        
        class AnimHumanAxeAttackAction : public AnimationBase, public Singleton<AnimHumanAxeAttackAction>
        {
            
        private:
            
            friend class Singleton<AnimHumanAxeAttackAction>;
            virtual ~AnimHumanAxeAttackAction() = default;
            
            AnimHumanAxeAttackAction()
            {
                _startFrame     = 0;
                _maxFrame       = 7;
                _fileName       = "HumanAxeAttackAction";
                _priority       = 3;
                _frameSwapTime  = 0.07f;
                _isReversePlay  = false;
            }
            
        };
        
    }
}









