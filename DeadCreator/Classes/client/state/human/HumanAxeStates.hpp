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

        };
        
        
        //
        // HumanAxeAttackReady
        //
        class HumanAxeAttackReady : public State<EntityPlayer>, public Singleton<HumanAxeAttackReady>
        {
            
        public:
            
            virtual void enter(EntityPlayer* human) override;
            virtual void execute(EntityPlayer* human) override;
            virtual void exit(EntityPlayer* human) override;
            
        };
        
        
        //
        // HumanAxeAttackRelease
        //
        class HumanAxeAttackRelease : public State<EntityPlayer>, public Singleton<HumanAxeAttackRelease>
        {
            
        public:
            
            virtual void enter(EntityPlayer* human) override;
            virtual void execute(EntityPlayer* human) override;
            virtual void exit(EntityPlayer* human) override;
            
        };
        
        
        //
        // HumanAxeAttackHover
        //
        class HumanAxeAttackHover : public State<EntityPlayer>, public Singleton<HumanAxeAttackHover>
        {
            
        public:
            
            virtual void enter(EntityPlayer* human) override;
            virtual void execute(EntityPlayer* human) override;
            virtual void exit(EntityPlayer* human) override;
            
        };
        
        
        //
        // HumanAxeAttackAction
        //
        class HumanAxeAttackAction : public State<EntityPlayer>, public Singleton<HumanAxeAttackAction>
        {
            
        public:
            
            virtual void enter(EntityPlayer* human) override;
            virtual void execute(EntityPlayer* human) override;
            virtual void exit(EntityPlayer* human) override;
            
        };
        
    }
}









