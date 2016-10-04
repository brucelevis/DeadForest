//
//  Zombie2Animations.hpp
//  DeadCreator
//
//  Created by mac on 2016. 10. 4..
//
//

#pragma once

#include "AnimationBase.hpp"
#include "Singleton.hpp"

namespace realtrick
{
    namespace client
    {
        
        class AnimZombie2IdleLoop : public AnimationBase, public Singleton<AnimZombie2IdleLoop>
        {
            
        private:
            
            friend class Singleton<AnimZombie2IdleLoop>;
            virtual ~AnimZombie2IdleLoop() = default;
            
            AnimZombie2IdleLoop()
            {
                _startFrame     = 0;
                _maxFrame       = 16;
                _fileName       = "Zombie2Breath";
                _priority       = 0;
                _frameSwapTime  = 0.1f;
                _isReversePlay  = false;
            }
            
        };
        
        
        class AnimZombie2RunLoop : public AnimationBase, public Singleton<AnimZombie2RunLoop>
        {
            
        private:
            
            friend class Singleton<AnimZombie2RunLoop>;
            virtual ~AnimZombie2RunLoop() = default;
            
            AnimZombie2RunLoop()
            {
                _startFrame     = 0;
                _maxFrame       = 12;
                _fileName       = "Zombie2RunLoop";
                _priority       = 4;
                _frameSwapTime  = 0.07f;
                _isReversePlay  = true;
            }
            
        };
        
        
        class AnimZombie2Dead : public AnimationBase, public Singleton<AnimZombie2Dead>
        {
            
        private:
            
            friend class Singleton<AnimZombie2Dead>;
            virtual ~AnimZombie2Dead() = default;
            
            AnimZombie2Dead()
            {
                _startFrame     = 0;
                _maxFrame       = 14;
                _fileName       = "Zombie2Death";
                _priority       = 4;
                _frameSwapTime  = 0.1f;
                _isReversePlay  = false;
            }
            
        };
        
        
        class AnimZombie2Attack1 : public AnimationBase, public Singleton<AnimZombie2Attack1>
        {
            
        private:
            
            friend class Singleton<AnimZombie2Attack1>;
            virtual ~AnimZombie2Attack1() = default;
            
            AnimZombie2Attack1()
            {
                _startFrame     = 0;
                _maxFrame       = 8;
                _fileName       = "Zombie2Attack1_";
                _priority       = 4;
                _frameSwapTime  = 0.1f;
                _isReversePlay  = false;
            }
            
        };
        
        
        class AnimZombie2Attack2 : public AnimationBase, public Singleton<AnimZombie2Attack2>
        {
            
        private:
            
            friend class Singleton<AnimZombie2Attack2>;
            virtual ~AnimZombie2Attack2() = default;
            
            AnimZombie2Attack2()
            {
                _startFrame     = 0;
                _maxFrame       = 8;
                _fileName       = "Zombie2Attack2_";
                _priority       = 4;
                _frameSwapTime  = 0.1f;
                _isReversePlay  = false;
            }
            
        };
        
    }
}









