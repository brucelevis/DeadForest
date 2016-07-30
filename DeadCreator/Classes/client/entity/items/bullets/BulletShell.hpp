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
            
            static BulletShell* create(GameManager* mgr);
            
            virtual BulletShell* clone() const override;
            
            virtual void discard() override;
            
        private:
            
            BulletShell(GameManager* mgr);
            
            virtual ~BulletShell();
            
        };
        
    }
}









