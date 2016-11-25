//
//  HumanGlock17States.hpp
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
        // HumanGlock17IdleLoop
        //
        class HumanGlock17IdleLoop : public State, public Singleton<HumanGlock17IdleLoop>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            virtual bool onMessage(HumanBase* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanGlock17IdleLoop>;
            HumanGlock17IdleLoop() = default;
            virtual ~HumanGlock17IdleLoop() = default;
            
        };
        
        
        //
        // HumanGlock17Out
        //
        class HumanGlock17Out : public State, public Singleton<HumanGlock17Out>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            virtual bool onMessage(HumanBase* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanGlock17Out>;
            HumanGlock17Out() = default;
            virtual ~HumanGlock17Out() = default;
            
        };
        
        
        //
        // HumanGlock17In
        //
        class HumanGlock17In : public State, public Singleton<HumanGlock17In>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            virtual bool onMessage(HumanBase* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanGlock17In>;
            HumanGlock17In() = default;
            virtual ~HumanGlock17In() = default;
            
        };
        
        
        //
        // HumanGlock17MoveLoop
        //
        class HumanGlock17MoveLoop : public State, public Singleton<HumanGlock17MoveLoop>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            virtual bool onMessage(HumanBase* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanGlock17MoveLoop>;
            HumanGlock17MoveLoop() = default;
            virtual ~HumanGlock17MoveLoop() = default;
            
            
        };
        
        
        //
        // HumanGlock17Attack
        //
        class HumanGlock17Attack : public State, public Singleton<HumanGlock17Attack>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            virtual bool onMessage(HumanBase* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanGlock17Attack>;
            HumanGlock17Attack() = default;
            virtual ~HumanGlock17Attack() = default;
            
        };
        
        
        //
        // HumanGlock17Reload
        //
        class HumanGlock17Reload : public State, public Singleton<HumanGlock17Reload>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            virtual bool onMessage(HumanBase* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanGlock17Reload>;
            HumanGlock17Reload() = default;
            virtual ~HumanGlock17Reload() = default;
            
        };
        
    }
}









