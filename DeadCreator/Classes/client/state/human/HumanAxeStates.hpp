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
        // HumanAxeIdleLoop
        //
        class HumanAxeIdleLoop : public State, public Singleton<HumanAxeIdleLoop>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            
        };
        
        
        //
        // HumanAxeOut
        //
        class HumanAxeOut : public State, public Singleton<HumanAxeOut>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            
        };
        
        
        //
        // HumanAxeIn
        //
        class HumanAxeIn : public State, public Singleton<HumanAxeIn>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            
        };
        
        
        //
        // HumanAxeMoveLoop
        //
        class HumanAxeMoveLoop : public State, public Singleton<HumanAxeMoveLoop>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;

        };
        
        
        //
        // HumanAxeAttackReady
        //
        class HumanAxeAttackReady : public State, public Singleton<HumanAxeAttackReady>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            
        };
        
        
        //
        // HumanAxeAttackRelease
        //
        class HumanAxeAttackRelease : public State, public Singleton<HumanAxeAttackRelease>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            
        };
        
        
        //
        // HumanAxeAttackHover
        //
        class HumanAxeAttackHover : public State, public Singleton<HumanAxeAttackHover>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            
        };
        
        
        //
        // HumanAxeAttackAction
        //
        class HumanAxeAttackAction : public State, public Singleton<HumanAxeAttackAction>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            
        };
        
    }
}









