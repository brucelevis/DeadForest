//
//  Bullet556mm.hpp
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
        
        class Bullet556mm : public BulletBase
        {
            
        public:
            
            explicit Bullet556mm(GameManager* mgr);
            virtual ~Bullet556mm();
            static Bullet556mm* create(GameManager* mgr);
            virtual Bullet556mm* clone() const override;
            virtual void discard() override;
            
        };
        
    }
}









