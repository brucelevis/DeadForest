//
//  BulletShell.hpp
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
        
        class BulletShell : public BulletBase
        {
            
        public:
            
            explicit BulletShell(Game* game);
            virtual ~BulletShell();
            static BulletShell* create(Game* game);
            virtual BulletShell* clone() const override;
            virtual void discard() override;
            
        };
        
    }
}









