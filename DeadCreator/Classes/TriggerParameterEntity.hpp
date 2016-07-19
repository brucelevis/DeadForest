//
//  TriggerParameterEntity.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 19..
//
//

#pragma once

#include "TriggerParameterBase.hpp"
#include "EditorEntityBase.hpp"

namespace realtrick
{
    
    class TriggerParameterEntity : public TriggerParameterBase
    {
        
    public:
        
        TriggerParameterEntity() : TriggerParameterBase(),
        _entity(nullptr)
        {
            _parameterType = TriggerParameterBase::Type::ENTITY;
            setParameterName("#invalid");
        }
        
        EditorEntityBase* getEntity() const { return _entity; }
        void setEntity(EditorEntityBase* entity)
        {
            _entity = entity;
            setParameterName(entity->getName());
        }
        
    private:
        
        EditorEntityBase* _entity;
        
    };
    
}