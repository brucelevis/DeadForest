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
        
        TriggerParameterEntity(const TriggerParameterEntity& rhs) { copyFrom(rhs); }
        TriggerParameterEntity& operator=(const TriggerParameterEntity& rhs)
        {
            if ( &rhs != this ) copyFrom(rhs);
            return *this;
        }
        
        void copyFrom(const TriggerParameterEntity& rhs)
        {
            TriggerParameterBase::copyFrom(rhs);
            _entityType = rhs._entityType;
        }
        
        virtual ~TriggerParameterEntity() = default;
        
        EntityType getEntityType() const { return _entityType; }
        void setEntityType(EntityType entityType)
        {
            _entityType = entityType;
            setParameterName(EditorEntity::getEntityTableByType().at(entityType).entityName);
        }
        
        virtual TriggerParameterEntity* clone() const override
        {
            return new TriggerParameterEntity(*this);
        }
        
    private:
        
        EntityType _entityType;
        
    };
    
}









