//
//  ZombieStates.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 2..
//
//

#pragma once

#include "Singleton.hpp"
#include "State.hpp"

namespace realtrick
{
    namespace client
    {
        
        class Telegram;
        class HumanBase;
        
        //
        // ZombieIdleLoop
        //
        class ZombieIdleLoop : public State, public Singleton<ZombieIdleLoop>
        {
            
        public:
            
            friend Singleton<ZombieIdleLoop>;
            ZombieIdleLoop() = default;
            virtual ~ZombieIdleLoop() = default;
            
            virtual void enter(HumanBase* zombie) override;
            virtual void execute(HumanBase* zombie) override;
            virtual void exit(HumanBase* zombie) override;
            virtual bool onMessage(HumanBase* zombie, const Telegram& msg) override;
            
        };
        
        
        //
        // ZombieRunLoop
        //
        class ZombieRunLoop : public State, public Singleton<ZombieRunLoop>
        {
            
        public:
            
            friend Singleton<ZombieRunLoop>;
            ZombieRunLoop() = default;
            virtual ~ZombieRunLoop() = default;
            
            virtual void enter(HumanBase* zombie) override;
            virtual void execute(HumanBase* zombie) override;
            virtual void exit(HumanBase* zombie) override;
            virtual bool onMessage(HumanBase* zombie, const Telegram& msg) override;
            
        };
        
        
        //
        // ZombieAttack1
        //
        class ZombieAttack1 : public State, public Singleton<ZombieAttack1>
        {
            
        public:
            
            friend Singleton<ZombieAttack1>;
            ZombieAttack1() = default;
            virtual ~ZombieAttack1() = default;
            
            virtual void enter(HumanBase* zombie) override;
            virtual void execute(HumanBase* zombie) override;
            virtual void exit(HumanBase* zombie) override;
            virtual bool onMessage(HumanBase* zombie, const Telegram& msg) override;
            
        };
        
        
        //
        // ZombieAttack2
        //
        class ZombieAttack2 : public State, public Singleton<ZombieAttack2>
        {
            
        public:
            
            friend Singleton<ZombieAttack2>;
            ZombieAttack2() = default;
            virtual ~ZombieAttack2() = default;
            
            virtual void enter(HumanBase* zombie) override;
            virtual void execute(HumanBase* zombie) override;
            virtual void exit(HumanBase* zombie) override;
            virtual bool onMessage(HumanBase* zombie, const Telegram& msg) override;
            
        };
        
        
        //
        // ZombieDead
        //
        class ZombieDead : public State, public Singleton<ZombieDead>
        {
            
        public:
            
            friend Singleton<ZombieDead>;
            ZombieDead() = default;
            virtual ~ZombieDead() = default;
            
            virtual void enter(HumanBase* zombie) override;
            virtual void execute(HumanBase* zombie) override;
            virtual void exit(HumanBase* zombie) override;
            virtual bool onMessage(HumanBase* zombie, const Telegram& msg) override;
            
        };
        
    }
}









