//
//  TriggerParameterEntity.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 19..
//
//

#pragma once

#include "TriggerParameterBase.hpp"
#include "EditorEntity.hpp"

namespace realtrick
{
    
    class TriggerParameterEntity : public TriggerParameterBase
    {
        
    public:
        
        TriggerParameterEntity() : TriggerParameterBase(),
        _entityType(EntityType::DEFAULT)
        {
            _parameterType = TriggerParameterBase::Type::ENTITY;
            setParameterName("#invalid");
        }
        
        EntityType getEntityType() const { return _entityType; }
        void setEntityType(EntityType entityType)
        {
            _entityType = entityType;
            setParameterName(EditorEntity::getEntityTableByType().at(entityType).entityName);
        }
        
    private:
        
        EntityType _entityType;
        
    };
    
}