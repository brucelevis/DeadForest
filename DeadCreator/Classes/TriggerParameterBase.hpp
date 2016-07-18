//
//  TriggerParameterBase.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 18..
//
//

#pragma once

#include <string>

namespace realtrick
{
    
    class TriggerParameterBase
    {
        
    public:
        
        enum class Type
        {
            INVALID = -1,
            PLAYER = 0,
            ENTITY,
            LOCATION,
            APPROXIMATION,
            NUMBER,
            STRING,
            SCORE,
            SWITCH,
            SWITCH_STATE,
            ORDER
        };
        
        TriggerParameterBase() :
        _parameterType(Type::INVALID),
        _parameterName("")
        {}
        
        Type getType() const { return _parameterType; }
        std::string getParameterName() const { return _parameterName; }
        void setParameterName(const std::string& name) { _parameterName = name; }
        
    protected:
        
        Type _parameterType;
        std::string _parameterName;
        
    };
    
    
}