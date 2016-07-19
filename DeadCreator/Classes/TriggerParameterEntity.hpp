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
        _entity(nullptr)
        {
            _parameterType = TriggerParameterBase::Type::ENTITY;
            setParameterName("#invalid");
        }
        
        EditorEntity* getEntity() const { return _entity; }
        void setEntity(EditorEntity* entity)
        {
            _entity = entity;
            setParameterName(entity->getName());
        }
        
    private:
        
        EditorEntity* _entity;
        
    };
    
}