//
//  EntityZombie2.hpp
//  DeadCreator
//
//  Created by mac on 2016. 10. 4..
//
//

#pragma once

#include "HumanBase.hpp"

namespace realtrick
{
    namespace client
    {
        
        class EntityZombie2 : public HumanBase
        {
            
        public:
            
            explicit EntityZombie2(Game* game);
            virtual ~EntityZombie2();
            
            virtual bool init() override;
            static EntityZombie2* create(Game* game);
            
            virtual bool handleMessage(const Telegram& msg) override;
            
            virtual void update(float dt) override;
            virtual void suicide() override;
            
        private:
            
            /* ... */
            
        };
        
    }
}
