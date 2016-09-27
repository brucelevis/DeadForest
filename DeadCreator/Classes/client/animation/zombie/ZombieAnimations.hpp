//
//  ZombieIdleAnimations.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 2..
//
//

#pragma once

#include "AnimationBase.hpp"
#include "Singleton.hpp"

namespace realtrick
{
    namespace client
    {
        
        class AnimZombieIdleLoop : public AnimationBase, public Singleton<AnimZombieIdleLoop>
        {
            
        private:
            
            friend class Singleton<AnimZombieIdleLoop>;
            virtual ~AnimZombieIdleLoop() = default;
            
            AnimZombieIdleLoop()
            {
                _startFrame     = 0;
                _maxFrame       = 18;
                _fileName       = "ZombieBreath";
                _priority       = 0;
                _frameSwapTime  = 0.1f;
                _isReversePlay  = false;
            }
            
        };
        
        
        class AnimZombieRunLoop : public AnimationBase, public Singleton<AnimZombieRunLoop>
        {
            
        private:
            
            friend class Singleton<AnimZombieRunLoop>;
            virtual ~AnimZombieRunLoop() = default;
            
            AnimZombieRunLoop()
            {
                _startFrame     = 0;
                _maxFrame       = 14;
                _fileName       = "ZombieRunLoop";
                _priority       = 4;
                _frameSwapTime  = 0.07f;
                _isReversePlay  = true;
            }
            
        };
        
        
        class AnimZombieDead : public AnimationBase, public Singleton<AnimZombieDead>
        {
            
        private:
            
            friend class Singleton<AnimZombieDead>;
            virtual ~AnimZombieDead() = default;
            
            AnimZombieDead()
            {
                _startFrame     = 0;
                _maxFrame       = 26;
                _fileName       = "ZombieDeath";
                _priority       = 4;
                _frameSwapTime  = 0.05f;
                _isReversePlay  = false;
            }
            
        };
        
        
        class AnimZombieAttack1 : public AnimationBase, public Singleton<AnimZombieAttack1>
        {
            
        private:
            
            friend class Singleton<AnimZombieAttack1>;
            virtual ~AnimZombieAttack1() = default;
            
            AnimZombieAttack1()
            {
                _startFrame     = 0;
                _maxFrame       = 9;
                _fileName       = "ZombieAttack1_";
                _priority       = 4;
                _frameSwapTime  = 0.05f;
                _isReversePlay  = false;
            }
            
        };
        
        
        class AnimZombieAttack2 : public AnimationBase, public Singleton<AnimZombieAttack2>
        {
            
        private:
            
            friend class Singleton<AnimZombieAttack2>;
            virtual ~AnimZombieAttack2() = default;
            
            AnimZombieAttack2()
            {
                _startFrame     = 0;
                _maxFrame       = 8;
                _fileName       = "ZombieAttack2_";
                _priority       = 4;
                _frameSwapTime  = 0.05f;
                _isReversePlay  = false;
            }
            
        };
        
    }
}









