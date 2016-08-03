//
//  HumanM1897Animations.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 1. 20..
//
//

#pragma once

#include "AnimationBase.hpp"
#include "Singleton.hpp"

namespace realtrick
{
    namespace client
    {
        
        class AnimHumanM1897IdleLoop : public AnimationBase, public Singleton<AnimHumanM1897IdleLoop>
        {
            
        private:
            
            friend class Singleton<AnimHumanM1897IdleLoop>;
            virtual ~AnimHumanM1897IdleLoop() = default;
            
            AnimHumanM1897IdleLoop()
            {
                _startFrame     = 0;
                _maxFrame       = 20;
                _fileName       = "HumanM1897IdleLoop";
                _priority       = 0;
                _frameSwapTime  = 0.1f;
                _isReversePlay  = false;
            }
            
        };
        
        
        class AnimHumanM1897In : public AnimationBase, public Singleton<AnimHumanM1897In>
        {
            
        private:
            
            friend class Singleton<AnimHumanM1897In>;
            virtual ~AnimHumanM1897In() = default;
            
            AnimHumanM1897In()
            {
                _startFrame     = 0;
                _maxFrame       = 5;
                _fileName       = "HumanM1897In";
                _priority       = 0;
                _frameSwapTime  = 0.1f;
                _isReversePlay  = false;
            }
            
        };
        
        
        class AnimHumanM1897Out : public AnimationBase, public Singleton<AnimHumanM1897Out>
        {
            
        private:
            
            friend class Singleton<AnimHumanM1897Out>;
            virtual ~AnimHumanM1897Out() = default;
            
            AnimHumanM1897Out()
            {
                _startFrame     = 0;
                _maxFrame       = 5;
                _fileName       = "HumanM1897In";
                _priority       = 0;
                _frameSwapTime  = 0.1f;
                _isReversePlay  = true;
            }
            
        };
        
        
        class AnimHumanM1897MoveLoop : public AnimationBase, public Singleton<AnimHumanM1897MoveLoop>
        {
            
        private:
            
            friend class Singleton<AnimHumanM1897MoveLoop>;
            virtual ~AnimHumanM1897MoveLoop() = default;
            
            AnimHumanM1897MoveLoop()
            {
                _startFrame     = 0;
                _maxFrame       = 16;
                _fileName       = "HumanM1897MoveLoop";
                _priority       = 3;
                _frameSwapTime  = 0.05f;
                _isReversePlay  = false;
            }
            
        };
        
        
        class AnimHumanM1897AttackReady : public AnimationBase, public Singleton<AnimHumanM1897AttackReady>
        {
            
        private:
            
            friend class Singleton<AnimHumanM1897AttackReady>;
            virtual ~AnimHumanM1897AttackReady() = default;
            
            AnimHumanM1897AttackReady()
            {
                _startFrame     = 0;
                _maxFrame       = 4;
                _fileName       = "HumanM1897AttackReady";
                _priority       = 3;
                _frameSwapTime  = 0.06f;
                _isReversePlay  = false;
            }
            
        };
        
        
        class AnimHumanM1897AttackRelease : public AnimationBase, public Singleton<AnimHumanM1897AttackRelease>
        {
            
        private:
            
            friend class Singleton<AnimHumanM1897AttackRelease>;
            virtual ~AnimHumanM1897AttackRelease() = default;
            
            AnimHumanM1897AttackRelease()
            {
                _startFrame     = 0;
                _maxFrame       = 4;
                _fileName       = "HumanM1897AttackReady";
                _priority       = 3;
                _frameSwapTime  = 0.06f;
                _isReversePlay  = true;
            }
            
        };
        
        
        class AnimHumanM1897AttackHover : public AnimationBase, public Singleton<AnimHumanM1897AttackHover>
        {
            
        private:
            
            friend class Singleton<AnimHumanM1897AttackHover>;
            virtual ~AnimHumanM1897AttackHover() = default;
            
            AnimHumanM1897AttackHover()
            {
                _startFrame     = 0;
                _maxFrame       = 2;
                _fileName       = "HumanM1897AttackHover";
                _priority       = 3;
                _frameSwapTime  = 0.06f;
                _isReversePlay  = false;
            }
            
        };
        
        
        class AnimHumanM1897AttackAction : public AnimationBase, public Singleton<AnimHumanM1897AttackAction>
        {
            
        private:
            
            friend class Singleton<AnimHumanM1897AttackAction>;
            virtual ~AnimHumanM1897AttackAction() = default;
            
            AnimHumanM1897AttackAction()
            {
                _startFrame     = 0;
                _maxFrame       = 13;
                _fileName       = "HumanM1897AttackAction";
                _priority       = 3;
                _frameSwapTime  = 0.06f;
                _isReversePlay  = false;
            }
            
        };
        
        
        class AnimHumanM1897Reload : public AnimationBase, public Singleton<AnimHumanM1897Reload>
        {
            
        private:
            
            friend class Singleton<AnimHumanM1897Reload>;
            virtual ~AnimHumanM1897Reload() = default;
            
            AnimHumanM1897Reload()
            {
                _startFrame     = 0;
                _maxFrame       = 9;
                _fileName       = "HumanM1897Reload";
                _priority       = 3;
                _frameSwapTime  = 0.14f;
                _isReversePlay  = false;
            }
            
        };
    }
}









