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
        
        TriggerParameterLocation(const TriggerParameterLocation& rhs) : TriggerParameterBase(rhs)
        {
            _location = rhs._location;
        }
        
        LocationNode* getLocation() const { return _location; }
        void setLocation(LocationNode* location)
        {
            _location = location;
            setParameterName(location->getLocationName());
        }
        
        virtual std::string getParameterName() const override
        {
            std::string ret = "#invalid";
            if ( _location ) ret = _location->getLocationName();
            return ret;
        }
        
        virtual TriggerParameterLocation* clone() const override
        {
            return new TriggerParameterLocation(*this);
        }
        
    private:
        
        LocationNode* _location;
        
    };
    
}









