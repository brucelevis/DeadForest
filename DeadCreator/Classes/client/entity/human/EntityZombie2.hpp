//
//  EntityZombie2.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 10. 4..
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
            
            virtual void suicide() override;
            
        private:
            
            /* ... */
            
        };
        
    }
}
