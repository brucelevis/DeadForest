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
        // HumanFistIdleLoop
        //
        class HumanFistIdleLoop : public State, public Singleton<HumanFistIdleLoop>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            
        };
        
        
        //
        // HumanFistOut
        //
        class HumanFistOut : public State, public Singleton<HumanFistOut>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            
        };
        
        
        //
        // HumanFistIn
        //
        class HumanFistIn : public State, public Singleton<HumanFistIn>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            
        };
        
        
        //
        // HumanFistMoveLoop
        //
        class HumanFistMoveLoop : public State, public Singleton<HumanFistMoveLoop>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            
        };
        
        
        //
        // HumanFistAttackReady
        //
        class HumanFistAttackReady : public State, public Singleton<HumanFistAttackReady>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            
        };
        
        
        //
        // HumanFistAttackRelease
        //
        class HumanFistAttackRelease : public State, public Singleton<HumanFistAttackRelease>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            
        };
        
        
        //
        // HumanFistAttackHover
        //
        class HumanFistAttackHover : public State, public Singleton<HumanFistAttackHover>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            
        };
        
        
        //
        // HumanFistAttackAction
        //
        class HumanFistAttackAction : public State, public Singleton<HumanFistAttackAction>
        {
            
        public:
            
            virtual void enter(HumanBase* human) override;
            virtual void execute(HumanBase* human) override;
            virtual void exit(HumanBase* human) override;
            
        };
        
    }
}









