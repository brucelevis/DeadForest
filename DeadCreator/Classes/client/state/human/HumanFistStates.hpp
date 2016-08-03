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
            
        };
        
        
        //
        // HumanFistAttackReady
        //
        class HumanFistAttackReady : public State<EntityPlayer>, public Singleton<HumanFistAttackReady>
        {
            
        public:
            
            virtual void enter(EntityPlayer* human) override;
            virtual void execute(EntityPlayer* human) override;
            virtual void exit(EntityPlayer* human) override;
            
        };
        
        
        //
        // HumanFistAttackRelease
        //
        class HumanFistAttackRelease : public State<EntityPlayer>, public Singleton<HumanFistAttackRelease>
        {
            
        public:
            
            virtual void enter(EntityPlayer* human) override;
            virtual void execute(EntityPlayer* human) override;
            virtual void exit(EntityPlayer* human) override;
            
        };
        
        
        //
        // HumanFistAttackHover
        //
        class HumanFistAttackHover : public State<EntityPlayer>, public Singleton<HumanFistAttackHover>
        {
            
        public:
            
            virtual void enter(EntityPlayer* human) override;
            virtual void execute(EntityPlayer* human) override;
            virtual void exit(EntityPlayer* human) override;
            
        };
        
        
        //
        // HumanFistAttackAction
        //
        class HumanFistAttackAction : public State<EntityPlayer>, public Singleton<HumanFistAttackAction>
        {
            
        public:
            
            virtual void enter(EntityPlayer* human) override;
            virtual void execute(EntityPlayer* human) override;
            virtual void exit(EntityPlayer* human) override;
            
        };
        
    }
}









