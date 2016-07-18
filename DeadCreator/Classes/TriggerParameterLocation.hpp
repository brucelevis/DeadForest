//
//  TriggerParameterLocation.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 19..
//
//

#pragma once

#include "TriggerParameterBase.hpp"
#include "LocationNode.hpp"

namespace realtrick
{
    
    class TriggerParameterLocation : public TriggerParameterBase
    {
        
    public:
        
        explicit TriggerParameterLocation(LocationNode* location) :
        TriggerParameterBase(),
        _location(location)
        {
            _parameterType = TriggerParameterBase::Type::ENTITY;
        }
        
        LocationNode* getLocation() const { return _location; }
        void setLocation(LocationNode* location) { _location = location; }
        
    private:
        
        LocationNode* _location;
        
    };
    
}