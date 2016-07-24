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
    
    class Bullet9mm : public BulletBase
    {
        
    public:
        
        static Bullet9mm* create(GameManager* mgr);
        
        virtual Bullet9mm* clone() const override;
        
        virtual void discard() override;
        
    private:
        
        Bullet9mm(GameManager* mgr);
        
        virtual ~Bullet9mm();
        
    };
    
}







