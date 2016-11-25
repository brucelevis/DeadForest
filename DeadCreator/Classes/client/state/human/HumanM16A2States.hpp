//
//  HumanM16A2States.hpp
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
        // HumanM16A2IdleLoop
        //
        class HumanM16A2IdleLoop : public State, public Singleton<HumanM16A2IdleLoop>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            virtual bool onMessage(HumanBase* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanM16A2IdleLoop>;
            HumanM16A2IdleLoop() = default;
            virtual ~HumanM16A2IdleLoop() = default;
            
        };
        
        
        //
        // HumanM16A2Out
        //
        class HumanM16A2Out : public State, public Singleton<HumanM16A2Out>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            virtual bool onMessage(HumanBase* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanM16A2Out>;
            HumanM16A2Out() = default;
            virtual ~HumanM16A2Out() = default;
            
        };
        
        
        //
        // HumanM16A2In
        //
        class HumanM16A2In : public State, public Singleton<HumanM16A2In>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            virtual bool onMessage(HumanBase* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanM16A2In>;
            HumanM16A2In() = default;
            virtual ~HumanM16A2In() = default;
            
        };
        
        
        //
        // HumanM16A2MoveLoop
        //
        class HumanM16A2MoveLoop : public State, public Singleton<HumanM16A2MoveLoop>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            virtual bool onMessage(HumanBase* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanM16A2MoveLoop>;
            HumanM16A2MoveLoop() = default;
            virtual ~HumanM16A2MoveLoop() = default;
            
            
        };
        
        
        //
        // HumanM16A2Attack
        //
        class HumanM16A2Attack : public State, public Singleton<HumanM16A2Attack>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            virtual bool onMessage(HumanBase* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanM16A2Attack>;
            HumanM16A2Attack() = default;
            virtual ~HumanM16A2Attack() = default;
            
        };
        
        
        //
        // HumanM16A2Reload
        //
        class HumanM16A2Reload : public State, public Singleton<HumanM16A2Reload>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            virtual bool onMessage(HumanBase* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanM16A2Reload>;
            HumanM16A2Reload() = default;
            virtual ~HumanM16A2Reload() = default;
            
        };
        
    }
}









