//
//  Zombie3Animations.hpp
//  DeadCreator
//
//  Created by mac on 2016. 10. 21..
//
//


#pragma once

#include "AnimationBase.hpp"
#include "Singleton.hpp"


namespace realtrick
{
    namespace client
    {
        
        class AnimZombie3IdleLoop : public AnimationBase, public Singleton<AnimZombie3IdleLoop>
        {
            
        private:
            
            friend class Singleton<AnimZombie3IdleLoop>;
            virtual ~AnimZombie3IdleLoop() = default;
            
            AnimZombie3IdleLoop()
            {
                _startFrame     = 0;
                _maxFrame       = 20;
                _fileName       = "Zombie3Breath";
                _priority       = 0;
                _frameSwapTime  = 0.1f;
                _isReversePlay  = false;
            }
            
        };
        
        
        class AnimZombie3RunLoop : public AnimationBase, public Singleton<AnimZombie3RunLoop>
        {
            
        private:
            
            friend class Singleton<AnimZombie3RunLoop>;
            virtual ~AnimZombie3RunLoop() = default;
            
            AnimZombie3RunLoop()
            {
                _startFrame     = 0;
                _maxFrame       = 10;
                _fileName       = "Zombie3Run";
                _priority       = 4;
                _frameSwapTime  = 0.07f;
                _isReversePlay  = true;
            }
            
        };
        
        
        class AnimZombie3Dead : public AnimationBase, public Singleton<AnimZombie3Dead>
        {
            
        private:
            
            friend class Singleton<AnimZombie3Dead>;
            virtual ~AnimZombie3Dead() = default;
            
            AnimZombie3Dead()
            {
                _startFrame     = 0;
                _maxFrame       = 11;
                _fileName       = "Zombie3Death";
                _priority       = 4;
                _frameSwapTime  = 0.1f;
                _isReversePlay  = false;
            }
            
        };
        
        
        class AnimZombie3Attack1 : public AnimationBase, public Singleton<AnimZombie3Attack1>
        {
            
        private:
            
            friend class Singleton<AnimZombie3Attack1>;
            virtual ~AnimZombie3Attack1() = default;
            
            AnimZombie3Attack1()
            {
                _startFrame     = 0;
                _maxFrame       = 8;
                _fileName       = "Zombie3Attack1_";
                _priority       = 4;
                _frameSwapTime  = 0.1f;
                _isReversePlay  = false;
            }
            
        };
        
        
        class AnimZombie3Attack2 : public AnimationBase, public Singleton<AnimZombie3Attack2>
        {
            
        private:
            
            friend class Singleton<AnimZombie3Attack2>;
            virtual ~AnimZombie3Attack2() = default;
            
            AnimZombie3Attack2()
            {
                _startFrame     = 0;
                _maxFrame       = 8;
                _fileName       = "Zombie3Attack2_";
                _priority       = 4;
                _frameSwapTime  = 0.1f;
                _isReversePlay  = false;
            }
            
        };
        
    }
}









