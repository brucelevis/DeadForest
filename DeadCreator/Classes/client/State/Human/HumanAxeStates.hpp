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
        // HumanAxeIdleLoop
        //
        class HumanAxeIdleLoop : public State<EntityHuman>, public Singleton<HumanAxeIdleLoop>
        {
            
        public:
            
            virtual void enter(EntityHuman* human) override;
            virtual void execute(EntityHuman* human) override;
            virtual void exit(EntityHuman* human) override;
            virtual bool onMessage(EntityHuman* human, const Telegram& msg) override;
            
        };
        
        
        
        //
        // HumanAxeOut
        //
        class HumanAxeOut : public State<EntityHuman>, public Singleton<HumanAxeOut>
        {
            
        public:
            
            virtual void enter(EntityHuman* human) override;
            virtual void execute(EntityHuman* human) override;
            virtual void exit(EntityHuman* human) override;
            virtual bool onMessage(EntityHuman* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanAxeOut>;
            HumanAxeOut() = default;
            virtual ~HumanAxeOut() = default;
            
        };
        
        
        
        //
        // HumanAxeIn
        //
        class HumanAxeIn : public State<EntityHuman>, public Singleton<HumanAxeIn>
        {
            
        public:
            
            virtual void enter(EntityHuman* human) override;
            virtual void execute(EntityHuman* human) override;
            virtual void exit(EntityHuman* human) override;
            virtual bool onMessage(EntityHuman* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanAxeIn>;
            HumanAxeIn() = default;
            virtual ~HumanAxeIn() = default;

        };
        
        
        
        //
        // HumanAxeMoveLoop
        //
        class HumanAxeMoveLoop : public State<EntityHuman>, public Singleton<HumanAxeMoveLoop>
        {
            
        public:
            
            virtual void enter(EntityHuman* human) override;
            virtual void execute(EntityHuman* human) override;
            virtual void exit(EntityHuman* human) override;
            virtual bool onMessage(EntityHuman* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanAxeMoveLoop>;
            HumanAxeMoveLoop() = default;
            virtual ~HumanAxeMoveLoop() = default;

            
        };
        
        
        
        //
        // HumanAxeAttack
        //
        class HumanAxeAttack : public State<EntityHuman>, public Singleton<HumanAxeAttack>
        {
            
        public:
            
            virtual void enter(EntityHuman* human) override;
            virtual void execute(EntityHuman* human) override;
            virtual void exit(EntityHuman* human) override;
            virtual bool onMessage(EntityHuman* human, const Telegram& msg) override;
          
        private:
            
            friend Singleton<HumanAxeAttack>;
            HumanAxeAttack() = default;
            virtual ~HumanAxeAttack() = default;
            
        };
        
    }
}









