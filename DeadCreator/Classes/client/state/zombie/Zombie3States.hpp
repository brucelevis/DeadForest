//
//  Zombie3States.hpp
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
        // Zombie3IdleLoop
        //
        class Zombie3IdleLoop : public State, public Singleton<Zombie3IdleLoop>
        {
            
        public:
            
            friend Singleton<Zombie3IdleLoop>;
            Zombie3IdleLoop() = default;
            virtual ~Zombie3IdleLoop() = default;
            
            virtual void enter(HumanBase* zombie) override;
            virtual void execute(HumanBase* zombie) override;
            virtual void exit(HumanBase* zombie) override;
            virtual bool onMessage(HumanBase* zombie, const Telegram& msg) override;
            
        };
        
        
        //
        // Zombie3RunLoop
        //
        class Zombie3RunLoop : public State, public Singleton<Zombie3RunLoop>
        {
            
        public:
            
            friend Singleton<Zombie3RunLoop>;
            Zombie3RunLoop() = default;
            virtual ~Zombie3RunLoop() = default;
            
            virtual void enter(HumanBase* zombie) override;
            virtual void execute(HumanBase* zombie) override;
            virtual void exit(HumanBase* zombie) override;
            virtual bool onMessage(HumanBase* zombie, const Telegram& msg) override;
            
        };
        
        
        //
        // Zombie3Attack1
        //
        class Zombie3Attack1 : public State, public Singleton<Zombie3Attack1>
        {
            
        public:
            
            friend Singleton<Zombie3Attack1>;
            Zombie3Attack1() = default;
            virtual ~Zombie3Attack1() = default;
            
            virtual void enter(HumanBase* zombie) override;
            virtual void execute(HumanBase* zombie) override;
            virtual void exit(HumanBase* zombie) override;
            virtual bool onMessage(HumanBase* zombie, const Telegram& msg) override;
            
        };
        
        
        //
        // Zombie3Attack2
        //
        class Zombie3Attack2 : public State, public Singleton<Zombie3Attack2>
        {
            
        public:
            
            friend Singleton<Zombie3Attack2>;
            Zombie3Attack2() = default;
            virtual ~Zombie3Attack2() = default;
            
            virtual void enter(HumanBase* zombie) override;
            virtual void execute(HumanBase* zombie) override;
            virtual void exit(HumanBase* zombie) override;
            virtual bool onMessage(HumanBase* zombie, const Telegram& msg) override;
            
        };
        
        
        //
        // Zombie3Dead
        //
        class Zombie3Dead : public State, public Singleton<Zombie3Dead>
        {
            
        public:
            
            friend Singleton<Zombie3Dead>;
            Zombie3Dead() = default;
            virtual ~Zombie3Dead() = default;
            
            virtual void enter(HumanBase* zombie) override;
            virtual void execute(HumanBase* zombie) override;
            virtual void exit(HumanBase* zombie) override;
            virtual bool onMessage(HumanBase* zombie, const Telegram& msg) override;
            
        };
        
    }
}









