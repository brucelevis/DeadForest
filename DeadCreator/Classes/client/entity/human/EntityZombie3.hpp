//
//  EntityZombie3.hpp
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
        
        class EntityZombie3 : public HumanBase
        {
            
        public:
            
            explicit EntityZombie3(Game* game);
            virtual ~EntityZombie3();
            
            virtual bool init() override;
            static EntityZombie3* create(Game* game);
            
            virtual bool handleMessage(const Telegram& msg) override;
            
            virtual void update(float dt) override;
            virtual void suicide() override;
            
        private:
            
            /* ... */
            
        };
        
    }
}
