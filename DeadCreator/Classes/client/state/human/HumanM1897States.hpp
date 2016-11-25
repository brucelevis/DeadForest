//
//  HumanM1897States.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 5. 14..
//
//

#pragma once

#include "State.hpp"
#include "Singleton.hpp"

namespace realtrick
{
    namespace client
    {
        
        class HumanBase;
        class Telegram;
        
        //
        // HumanM1897IdleLoop
        //
        class HumanM1897IdleLoop : public State, public Singleton<HumanM1897IdleLoop>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            virtual bool onMessage(HumanBase* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanM1897IdleLoop>;
            HumanM1897IdleLoop() = default;
            virtual ~HumanM1897IdleLoop() = default;
            
        };
        
        
        //
        // HumanM1897Out
        //
        class HumanM1897Out : public State, public Singleton<HumanM1897Out>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            virtual bool onMessage(HumanBase* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanM1897Out>;
            HumanM1897Out() = default;
            virtual ~HumanM1897Out() = default;
            
        };
        
        
        //
        // HumanM1897In
        //
        class HumanM1897In : public State, public Singleton<HumanM1897In>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            virtual bool onMessage(HumanBase* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanM1897In>;
            HumanM1897In() = default;
            virtual ~HumanM1897In() = default;
            
        };
        
        
        //
        // HumanM1897MoveLoop
        //
        class HumanM1897MoveLoop : public State, public Singleton<HumanM1897MoveLoop>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            virtual bool onMessage(HumanBase* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanM1897MoveLoop>;
            HumanM1897MoveLoop() = default;
            virtual ~HumanM1897MoveLoop() = default;
            
            
        };
        
        
        //
        // HumanM1897Reload
        //
        class HumanM1897Reload : public State, public Singleton<HumanM1897Reload>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            virtual bool onMessage(HumanBase* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanM1897Reload>;
            HumanM1897Reload() = default;
            virtual ~HumanM1897Reload() = default;
            
        };
     
        
        //
        // HumanM1897AttackReady
        //
        class HumanM1897AttackReady : public State, public Singleton<HumanM1897AttackReady>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            
        };
        
        
        //
        // HumanM1897AttackRelease
        //
        class HumanM1897AttackRelease : public State, public Singleton<HumanM1897AttackRelease>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            
        };
        
        
        //
        // HumanM1897AttackHover
        //
        class HumanM1897AttackHover : public State, public Singleton<HumanM1897AttackHover>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            
        };
        
        
        //
        // HumanM1897AttackAction
        //
        class HumanM1897AttackAction : public State, public Singleton<HumanM1897AttackAction>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            
        };
        
    }
}









