//
//  BulletBase.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 2. 3..
//
//

#include "BulletBase.hpp"
using namespace realtrick::client;

BulletBase::BulletBase(GameManager* mgr) : ItemBase(mgr)
{
    ADD_FAMILY_MASK(_familyMask, BULLET_BASE);
}

BulletBase::BulletBase(const BulletBase& rhs) : ItemBase(rhs)
{
    
}

BulletBase::~BulletBase()
{
    
}









