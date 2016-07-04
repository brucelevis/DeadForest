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


void EntityBase::visit(cocos2d::Renderer* renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags)
{
    setPosition( _worldPosition - _gmxLayer.getCameraPosition() + Vec2(_gmxLayer.getLayerSize() / 2) );
    Node::visit(renderer, parentTransform, parentFlags);
}