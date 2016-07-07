//
//  EditorEntityBase.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "EditorEntityBase.hpp"
#include "GMXLayer2.hpp"
using namespace cocos2d;
using namespace realtrick;

EditorEntityBase::EditorEntityBase(GMXLayer2& layer, int id) :
_gmxLayer(layer),
_id(id)
{
    setTag(id);
    _type = EntityType::INVALID;
}


EditorEntityBase::~EditorEntityBase()
{
}


void EditorEntityBase::setBoundingBox(const cocos2d::Rect& aabb)
{
    _boundingBox = aabb;
}


bool EditorEntityBase::init()
{
    if ( !Node::init() )
        return false;
    
    _selectedCircle = Sprite::create("circle.png");
    _selectedCircle->setVisible(false);
    addChild(_selectedCircle);
    
    return true;
}







