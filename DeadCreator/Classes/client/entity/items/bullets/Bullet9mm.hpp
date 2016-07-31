//
//  Bullet9mm.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 2. 3..
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
            
            explicit Bullet9mm(Game* game);
            virtual ~Bullet9mm();
            static Bullet9mm* create(Game* game);
            virtual Bullet9mm* clone() const override;
            virtual void discard() override;
            
        };
        
    }
}









