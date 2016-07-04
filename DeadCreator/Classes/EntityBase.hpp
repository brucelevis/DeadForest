//
//  EntityBase.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#pragma once

#include "cocos2d.h"

class GMXLayer2;

enum class EntityType : int
{
    INVALID = -1,
    SHERIFF = 0,
    ITEM_556MM,
    ITEM_9MM,
    ITEM_SHELL,
    ITEM_GLOCK17,
    ITEM_M16A2,
    ITEM_M1897,
    ITEM_AXE,
};

class EntityBase : public cocos2d::Node
{
    
public:
    
    EntityBase(GMXLayer2& layer, int id);
    
    virtual ~EntityBase();
    
    cocos2d::Vec2 getWorldPosition() const { return _worldPosition; }
    
    void setWorldPosition(const cocos2d::Vec2& pos) { _worldPosition = pos; }
    
    virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;
    
protected:
    
    GMXLayer2& _gmxLayer;
    
    cocos2d::Vec2 _worldPosition;
    
    int _id;
    
    EntityType _type;
    
    cocos2d::Sprite* _body;
    cocos2d::Sprite* _shadow;
    
};