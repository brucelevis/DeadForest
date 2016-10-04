//
//  Zombie2States.hpp
//  DeadCreator
//
//  Created by mac on 2016. 10. 4..
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
        // Zombie2IdleLoop
        //
        class Zombie2IdleLoop : public State, public Singleton<Zombie2IdleLoop>
        {
            
        public:
            
            friend Singleton<Zombie2IdleLoop>;
            Zombie2IdleLoop() = default;
            virtual ~Zombie2IdleLoop() = default;
            
            virtual void enter(HumanBase* zombie) override;
            virtual void execute(HumanBase* zombie) override;
            virtual void exit(HumanBase* zombie) override;
            virtual bool onMessage(HumanBase* zombie, const Telegram& msg) override;
            
        };
        
        
        //
        // Zombie2RunLoop
        //
        class Zombie2RunLoop : public State, public Singleton<Zombie2RunLoop>
        {
            
        public:
            
            friend Singleton<Zombie2RunLoop>;
            Zombie2RunLoop() = default;
            virtual ~Zombie2RunLoop() = default;
            
            virtual void enter(HumanBase* zombie) override;
            virtual void execute(HumanBase* zombie) override;
            virtual void exit(HumanBase* zombie) override;
            virtual bool onMessage(HumanBase* zombie, const Telegram& msg) override;
            
        };
        
        
        //
        // Zombie2Attack1
        //
        class Zombie2Attack1 : public State, public Singleton<Zombie2Attack1>
        {
            
        public:
            
            friend Singleton<Zombie2Attack1>;
            Zombie2Attack1() = default;
            virtual ~Zombie2Attack1() = default;
            
            virtual void enter(HumanBase* zombie) override;
            virtual void execute(HumanBase* zombie) override;
            virtual void exit(HumanBase* zombie) override;
            virtual bool onMessage(HumanBase* zombie, const Telegram& msg) override;
            
        };
        
        
        //
        // Zombie2Attack2
        //
        class Zombie2Attack2 : public State, public Singleton<Zombie2Attack2>
        {
            
        public:
            
            friend Singleton<Zombie2Attack2>;
            Zombie2Attack2() = default;
            virtual ~Zombie2Attack2() = default;
            
            virtual void enter(HumanBase* zombie) override;
            virtual void execute(HumanBase* zombie) override;
            virtual void exit(HumanBase* zombie) override;
            virtual bool onMessage(HumanBase* zombie, const Telegram& msg) override;
            
        };
        
        
        //
        // Zombie2Dead
        //
        class Zombie2Dead : public State, public Singleton<Zombie2Dead>
        {
            
        public:
            
            friend Singleton<Zombie2Dead>;
            Zombie2Dead() = default;
            virtual ~Zombie2Dead() = default;
            
            virtual void enter(HumanBase* zombie) override;
            virtual void execute(HumanBase* zombie) override;
            virtual void exit(HumanBase* zombie) override;
            virtual bool onMessage(HumanBase* zombie, const Telegram& msg) override;
            
        };
        
    }
}









