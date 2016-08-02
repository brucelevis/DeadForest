#pragma once

#include "State.hpp"
#include "Singleton.hpp"

namespace realtrick
{
    namespace client
    {
        
        class EntityPlayer;
        class Telegram;
        
        //
        // HumanAxeIdleLoop
        //
        class HumanAxeIdleLoop : public State<EntityPlayer>, public Singleton<HumanAxeIdleLoop>
        {
            
        public:
            
            virtual void enter(EntityPlayer* human) override;
            virtual void execute(EntityPlayer* human) override;
            virtual void exit(EntityPlayer* human) override;
            virtual bool onMessage(EntityPlayer* human, const Telegram& msg) override;
            
        };
        
        
        
        //
        // HumanAxeOut
        //
        class HumanAxeOut : public State<EntityPlayer>, public Singleton<HumanAxeOut>
        {
            
        public:
            
            virtual void enter(EntityPlayer* human) override;
            virtual void execute(EntityPlayer* human) override;
            virtual void exit(EntityPlayer* human) override;
            virtual bool onMessage(EntityPlayer* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanAxeOut>;
            HumanAxeOut() = default;
            virtual ~HumanAxeOut() = default;
            
        };
        
        
        
        //
        // HumanAxeIn
        //
        class HumanAxeIn : public State<EntityPlayer>, public Singleton<HumanAxeIn>
        {
            
        public:
            
            virtual void enter(EntityPlayer* human) override;
            virtual void execute(EntityPlayer* human) override;
            virtual void exit(EntityPlayer* human) override;
            virtual bool onMessage(EntityPlayer* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanAxeIn>;
            HumanAxeIn() = default;
            virtual ~HumanAxeIn() = default;

        };
        
        
        
        //
        // HumanAxeMoveLoop
        //
        class HumanAxeMoveLoop : public State<EntityPlayer>, public Singleton<HumanAxeMoveLoop>
        {
            
        public:
            
            virtual void enter(EntityPlayer* human) override;
            virtual void execute(EntityPlayer* human) override;
            virtual void exit(EntityPlayer* human) override;
            virtual bool onMessage(EntityPlayer* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanAxeMoveLoop>;
            HumanAxeMoveLoop() = default;
            virtual ~HumanAxeMoveLoop() = default;

        };
        
        
        
        //
        // HumanAxeAttack
        //
        class HumanAxeAttack : public State<EntityPlayer>, public Singleton<HumanAxeAttack>
        {
            
        public:
            
            virtual void enter(EntityPlayer* human) override;
            virtual void execute(EntityPlayer* human) override;
            virtual void exit(EntityPlayer* human) override;
            virtual bool onMessage(EntityPlayer* human, const Telegram& msg) override;
          
        private:
            
            friend Singleton<HumanAxeAttack>;
            HumanAxeAttack() = default;
            virtual ~HumanAxeAttack() = default;
            
        };
        
    }
}









