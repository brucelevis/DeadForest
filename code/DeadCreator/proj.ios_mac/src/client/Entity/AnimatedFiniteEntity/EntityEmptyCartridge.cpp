//
//  EntityEmptyCartridge.cpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 3. 5..
//
//

#include "EntityEmptyCartridge.hpp"

namespace realtrick
{
    
    EntityEmptyCartridge::EntityEmptyCartridge(GameManager* mgr) : AnimatedFiniteEntity(mgr)
    {
    }
    
    
    EntityEmptyCartridge::EntityEmptyCartridge(const EntityEmptyCartridge& rhs) : AnimatedFiniteEntity(rhs)
    {
        setEntityType(EntityType::ENTITY_EMPTY_CARTRIDGE);
    }
    
    
    EntityEmptyCartridge::~EntityEmptyCartridge()
    {
    }
    
    
    EntityEmptyCartridge* EntityEmptyCartridge::EntityEmptyCartridge::create(GameManager* mgr, const std::vector<std::string>& frames, float deathTime, cocos2d::ui::Widget::TextureResType type)
    {
        EntityEmptyCartridge* ret = new (std::nothrow) EntityEmptyCartridge(mgr);
        if ( ret && ret->init(frames, deathTime, type) )
        {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
    
    
    AnimatedFiniteEntity* EntityEmptyCartridge::clone() const
    {
        return new EntityEmptyCartridge(*this);
    }
    
}









