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
        
        class EntityHuman;
        class Telegram;
        
        //
        // HumanM1897IdleLoop
        //
        class HumanM1897IdleLoop : public State<EntityHuman>, public Singleton<HumanM1897IdleLoop>
        {
            
        public:
            
            virtual void enter(EntityHuman* human) override;
            virtual void execute(EntityHuman* human) override;
            virtual void exit(EntityHuman* human) override;
            virtual bool onMessage(EntityHuman* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanM1897IdleLoop>;
            HumanM1897IdleLoop() = default;
            virtual ~HumanM1897IdleLoop() = default;
            
        };
        
        
        //
        // HumanM1897Out
        //
        class HumanM1897Out : public State<EntityHuman>, public Singleton<HumanM1897Out>
        {
            
        public:
            
            virtual void enter(EntityHuman* human) override;
            virtual void execute(EntityHuman* human) override;
            virtual void exit(EntityHuman* human) override;
            virtual bool onMessage(EntityHuman* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanM1897Out>;
            HumanM1897Out() = default;
            virtual ~HumanM1897Out() = default;
            
        };
        
        
        //
        // HumanM1897In
        //
        class HumanM1897In : public State<EntityHuman>, public Singleton<HumanM1897In>
        {
            
        public:
            
            virtual void enter(EntityHuman* human) override;
            virtual void execute(EntityHuman* human) override;
            virtual void exit(EntityHuman* human) override;
            virtual bool onMessage(EntityHuman* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanM1897In>;
            HumanM1897In() = default;
            virtual ~HumanM1897In() = default;
            
        };
        
        
        //
        // HumanM1897MoveLoop
        //
        class HumanM1897MoveLoop : public State<EntityHuman>, public Singleton<HumanM1897MoveLoop>
        {
            
        public:
            
            virtual void enter(EntityHuman* human) override;
            virtual void execute(EntityHuman* human) override;
            virtual void exit(EntityHuman* human) override;
            virtual bool onMessage(EntityHuman* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanM1897MoveLoop>;
            HumanM1897MoveLoop() = default;
            virtual ~HumanM1897MoveLoop() = default;
            
            
        };
        
        
        //
        // HumanM1897Attack
        //
        class HumanM1897Attack : public State<EntityHuman>, public Singleton<HumanM1897Attack>
        {
            
        public:
            
            virtual void enter(EntityHuman* human) override;
            virtual void execute(EntityHuman* human) override;
            virtual void exit(EntityHuman* human) override;
            virtual bool onMessage(EntityHuman* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanM1897Attack>;
            HumanM1897Attack() = default;
            virtual ~HumanM1897Attack() = default;
            
        };
        
        
        //
        // HumanM1897Reload
        //
        class HumanM1897Reload : public State<EntityHuman>, public Singleton<HumanM1897Reload>
        {
            
        public:
            
            virtual void enter(EntityHuman* human) override;
            virtual void execute(EntityHuman* human) override;
            virtual void exit(EntityHuman* human) override;
            virtual bool onMessage(EntityHuman* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanM1897Reload>;
            HumanM1897Reload() = default;
            virtual ~HumanM1897Reload() = default;
            
        };
        
    }
}









