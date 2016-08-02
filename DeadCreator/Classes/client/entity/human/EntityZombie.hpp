//
//  EntityZombie.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 2..
//
//

#pragma once

#include "HumanBase.hpp"

namespace realtrick
{
    namespace client
    {
        
        class EntityZombie : public HumanBase
        {
            
        public:
            
            explicit EntityZombie(Game* game);
            virtual ~EntityZombie();
            
            virtual bool init() override;
            static EntityZombie* create(Game* game);
            
            virtual bool handleMessage(const Telegram& msg) override { return _FSM->handleMessage(msg); }
            
            StateMachine<EntityZombie>* getFSM() const { return _FSM; }
            
            virtual void update(float dt) override;
            
        protected:
            
            StateMachine<EntityZombie>*     _FSM;
            
        };
        
    }
}