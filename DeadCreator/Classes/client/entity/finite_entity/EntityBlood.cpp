//
//  EntityBlood.cpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 3. 2..
//
//

#include "EntityBlood.hpp"
#include "GameManager.hpp"
using namespace realtrick::client;
using namespace cocos2d;

EntityBlood* EntityBlood::create(GameManager* mgr,
                                 const std::string& fileName,
                                 float deathTime,
                                 cocos2d::ui::Widget::TextureResType type)
{
    EntityBlood* ret = new (std::nothrow) EntityBlood(mgr);
    if ( ret && ret->init(fileName, deathTime, type) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


EntityBlood::EntityBlood(GameManager* mgr) : FiniteEntityBase(mgr)
{
    setEntityType(ENTITY_BLOOD);
}


EntityBlood::EntityBlood(const EntityBlood& rhs) : FiniteEntityBase(rhs)
{
    
}


EntityBlood::~EntityBlood()
{
    
}


FiniteEntityBase* EntityBlood::clone() const
{
    return new EntityBlood(*this);
}









