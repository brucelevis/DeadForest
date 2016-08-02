//
//  ZombieStates.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 2..
//
//

#pragma once

#include "Singleton.hpp"
#include "State.hpp"

namespace realtrick
{
    namespace client
    {
        
        class Telegram;
        class EntityZombie;
        
        //
        // ZombieIdleLoop
        //
        class ZombieIdleLoop : public State<EntityZombie>, public Singleton<ZombieIdleLoop>
        {
            
        public:
            
            friend Singleton<ZombieIdleLoop>;
            ZombieIdleLoop() = default;
            virtual ~ZombieIdleLoop() = default;
            
            virtual void enter(EntityZombie* zombie) override;
            virtual void execute(EntityZombie* zombie) override;
            virtual void exit(EntityZombie* zombie) override;
            virtual bool onMessage(EntityZombie* zombie, const Telegram& msg) override;
            
        };
        
        
        //
        // ZombieRunLoop
        //
        class ZombieRunLoop : public State<EntityZombie>, public Singleton<ZombieRunLoop>
        {
            
        public:
            
            friend Singleton<ZombieRunLoop>;
            ZombieRunLoop() = default;
            virtual ~ZombieRunLoop() = default;
            
            virtual void enter(EntityZombie* zombie) override;
            virtual void execute(EntityZombie* zombie) override;
            virtual void exit(EntityZombie* zombie) override;
            virtual bool onMessage(EntityZombie* zombie, const Telegram& msg) override;
            
        };
        
        
        //
        // ZombieAttack
        //
        class ZombieAttack : public State<EntityZombie>, public Singleton<ZombieAttack>
        {
            
        public:
            
            friend Singleton<ZombieAttack>;
            ZombieAttack() = default;
            virtual ~ZombieAttack() = default;
            
            virtual void enter(EntityZombie* zombie) override;
            virtual void execute(EntityZombie* zombie) override;
            virtual void exit(EntityZombie* zombie) override;
            virtual bool onMessage(EntityZombie* zombie, const Telegram& msg) override;
            
        };
        
        
        //
        // ZombieDead
        //
        class ZombieDead : public State<EntityZombie>, public Singleton<ZombieDead>
        {
            
        public:
            
            friend Singleton<ZombieDead>;
            ZombieDead() = default;
            virtual ~ZombieDead() = default;
            
            virtual void enter(EntityZombie* zombie) override;
            virtual void execute(EntityZombie* zombie) override;
            virtual void exit(EntityZombie* zombie) override;
            virtual bool onMessage(EntityZombie* zombie, const Telegram& msg) override;
            
        };
        
    }
}









