//
//  EntityBase.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "EntityBase.hpp"
#include "GMXLayer2.hpp"
using namespace cocos2d;


EntityBase::EntityBase(GMXLayer2& layer, int id) :
_gmxLayer(layer),
_id(id)
{
    setTag(id);
    _type = EntityType::INVALID;
}


EntityBase::~EntityBase()
{
}