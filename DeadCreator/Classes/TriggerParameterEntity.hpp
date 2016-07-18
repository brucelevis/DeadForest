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
        
        explicit TriggerParameterEntity(EditorEntityBase* entity) :
        TriggerParameterBase(),
        _entity(entity)
        {
            _parameterType = TriggerParameterBase::Type::ENTITY;
        }
        
        EditorEntityBase* getEntity() const { return _entity; }
        void setEntity(EditorEntityBase* entity) { _entity = entity; }
        
    private:
        
        EditorEntityBase* _entity;
        
    };
    
}