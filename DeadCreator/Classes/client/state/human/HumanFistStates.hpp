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
        // HumanFistIdleLoop
        //
        class HumanFistIdleLoop : public State<EntityPlayer>, public Singleton<HumanFistIdleLoop>
        {
            
        public:
            
            virtual void enter(EntityPlayer* human) override;
            virtual void execute(EntityPlayer* human) override;
            virtual void exit(EntityPlayer* human) override;
            virtual bool onMessage(EntityPlayer* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanFistIdleLoop>;
            HumanFistIdleLoop() = default;
            virtual ~HumanFistIdleLoop() = default;
            
        };
        
        
        
        //
        // HumanFistOut
        //
        class HumanFistOut : public State<EntityPlayer>, public Singleton<HumanFistOut>
        {
            
        public:
            
            virtual void enter(EntityPlayer* human) override;
            virtual void execute(EntityPlayer* human) override;
            virtual void exit(EntityPlayer* human) override;
            virtual bool onMessage(EntityPlayer* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanFistOut>;
            HumanFistOut() = default;
            virtual ~HumanFistOut() = default;
            
        };
        
        
        
        //
        // HumanFistIn
        //
        class HumanFistIn : public State<EntityPlayer>, public Singleton<HumanFistIn>
        {
            
        public:
            
            virtual void enter(EntityPlayer* human) override;
            virtual void execute(EntityPlayer* human) override;
            virtual void exit(EntityPlayer* human) override;
            virtual bool onMessage(EntityPlayer* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanFistIn>;
            HumanFistIn() = default;
            virtual ~HumanFistIn() = default;
            
        };
        
        
        
        //
        // HumanFistMoveLoop
        //
        class HumanFistMoveLoop : public State<EntityPlayer>, public Singleton<HumanFistMoveLoop>
        {
            
        public:
            
            virtual void enter(EntityPlayer* human) override;
            virtual void execute(EntityPlayer* human) override;
            virtual void exit(EntityPlayer* human) override;
            virtual bool onMessage(EntityPlayer* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanFistMoveLoop>;
            HumanFistMoveLoop() = default;
            virtual ~HumanFistMoveLoop() = default;
            
        };
        
        
        
        //
        // HumanFistAttack
        //
        class HumanFistAttack : public State<EntityPlayer>, public Singleton<HumanFistAttack>
        {
            
        public:
            
            virtual void enter(EntityPlayer* human) override;
            virtual void execute(EntityPlayer* human) override;
            virtual void exit(EntityPlayer* human) override;
            virtual bool onMessage(EntityPlayer* human, const Telegram& msg) override;
            
        private:
            
            friend Singleton<HumanFistAttack>;
            HumanFistAttack() = default;
            virtual ~HumanFistAttack() = default;
            
        };
        
    }
}









