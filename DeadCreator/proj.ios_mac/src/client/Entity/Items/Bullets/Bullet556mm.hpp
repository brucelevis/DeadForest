//
//  Bullet556mm.hpp
//  TheDeadForest
//
//  Created by mac on 2016. 2. 3..
//
//

#pragma once

#include "BulletBase.hpp"

namespace realtrick
{
    
    class Bullet556mm : public BulletBase
    {
        
    public:
        
        static Bullet556mm* create(GameManager* mgr);
        
        virtual Bullet556mm* clone() const override;
        
        virtual void discard() override;
        
    private:
        
        Bullet556mm(GameManager* mgr);
        
        virtual ~Bullet556mm();
        
    };
    
}