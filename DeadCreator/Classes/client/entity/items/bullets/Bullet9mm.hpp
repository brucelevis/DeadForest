//
//  Bullet9mm.hpp
//  TheDeadForest
//
//  Created by mac on 2016. 2. 3..
//
//

#pragma once

#include "BulletBase.hpp"

namespace realtrick
{
    namespace client
    {
        
        class Bullet9mm : public BulletBase
        {
            
        public:
            
            explicit Bullet9mm(GameManager* mgr);
            virtual ~Bullet9mm();
            static Bullet9mm* create(GameManager* mgr);
            virtual Bullet9mm* clone() const override;
            virtual void discard() override;
            
        };
        
    }
}









