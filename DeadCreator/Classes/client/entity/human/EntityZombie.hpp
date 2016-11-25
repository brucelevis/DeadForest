//
//  EntityZombie.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 8. 2..
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
            
            virtual void suicide() override;
            
        private:
            
            /* ... */
            
        };
        
    }
}









