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
        
        TriggerParameterLocation() : TriggerParameterBase(),
        _location(nullptr)
        {
            _parameterType = TriggerParameterBase::Type::ENTITY;
            setParameterName("#invalid");
        }
        
        LocationNode* getLocation() const { return _location; }
        void setLocation(LocationNode* location)
        {
            _location = location;
            setParameterName(location->getLocationName());
        }
        
    private:
        
        LocationNode* _location;
        
    };
    
}