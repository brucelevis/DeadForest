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
        // HumanFistIdleLoop
        //
        class HumanFistIdleLoop : public State<EntityHuman>, public Singleton<HumanFistIdleLoop>
        {
            
        public:
            
            virtual void enter(EntityHuman* human) override;
            virtual void execute(EntityHuman* human) override;
            virtual void exit(EntityHuman* human) override;
            virtual bool onMessage(EntityHuman* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanFistIdleLoop>;
            HumanFistIdleLoop() = default;
            virtual ~HumanFistIdleLoop() = default;
            
        };
        
        
        
        //
        // HumanFistOut
        //
        class HumanFistOut : public State<EntityHuman>, public Singleton<HumanFistOut>
        {
            
        public:
            
            virtual void enter(EntityHuman* human) override;
            virtual void execute(EntityHuman* human) override;
            virtual void exit(EntityHuman* human) override;
            virtual bool onMessage(EntityHuman* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanFistOut>;
            HumanFistOut() = default;
            virtual ~HumanFistOut() = default;
            
        };
        
        
        
        //
        // HumanFistIn
        //
        class HumanFistIn : public State<EntityHuman>, public Singleton<HumanFistIn>
        {
            
        public:
            
            virtual void enter(EntityHuman* human) override;
            virtual void execute(EntityHuman* human) override;
            virtual void exit(EntityHuman* human) override;
            virtual bool onMessage(EntityHuman* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanFistIn>;
            HumanFistIn() = default;
            virtual ~HumanFistIn() = default;
            
        };
        
        
        
        //
        // HumanFistMoveLoop
        //
        class HumanFistMoveLoop : public State<EntityHuman>, public Singleton<HumanFistMoveLoop>
        {
            
        public:
            
            virtual void enter(EntityHuman* human) override;
            virtual void execute(EntityHuman* human) override;
            virtual void exit(EntityHuman* human) override;
            virtual bool onMessage(EntityHuman* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanFistMoveLoop>;
            HumanFistMoveLoop() = default;
            virtual ~HumanFistMoveLoop() = default;
            
        };
        
        
        
        //
        // HumanFistAttack
        //
        class HumanFistAttack : public State<EntityHuman>, public Singleton<HumanFistAttack>
        {
            
        public:
            
            virtual void enter(EntityHuman* human) override;
            virtual void execute(EntityHuman* human) override;
            virtual void exit(EntityHuman* human) override;
            virtual bool onMessage(EntityHuman* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanFistAttack>;
            HumanFistAttack() = default;
            virtual ~HumanFistAttack() = default;
            
        };
        
    }
}









