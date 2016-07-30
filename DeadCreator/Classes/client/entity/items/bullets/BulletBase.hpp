//
//  BulletBase.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 2. 3..
//
//

#pragma once

#include "ItemBase.hpp"

namespace realtrick
{
    namespace client
    {
        
        class BulletBase : public ItemBase
        {
            
        public:
            
            explicit BulletBase(GameManager* mgr);
            BulletBase(const BulletBase& rhs);
            virtual ~BulletBase();
            virtual ItemBase* clone() const override = 0;
            virtual void use() override {}
            virtual void discard() override = 0;
            
        };
        
    }
}









