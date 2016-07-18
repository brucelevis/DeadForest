//
//  TriggerParameterApproximation.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 19..
//
//

#pragma once

#include "TriggerParameterBase.hpp"

namespace realtrick
{
    
    class TriggerParameterApproximation : public TriggerParameterBase
    {
        
    public:
        
        enum class Type
        {
            INVALID = -1,
            AT_LEAST = 0,
            AT_MOST,
            EXACTLY
        };
        
    public:
        
        explicit TriggerParameterApproximation(Type type) :
        TriggerParameterBase(),
        _approximation(type)
        {
            _parameterType = TriggerParameterBase::Type::APPROXIMATION;
            
            if ( type == Type::AT_LEAST ) setParameterName("at least");
            else if ( type == Type::AT_MOST ) setParameterName("at most");
            else if ( type == Type::EXACTLY ) setParameterName("exactly");
        }
        
        Type getApproximationType() const { return _approximation; }
        void setApproximationType(Type type) { _approximation = type; }
        
    private:
        
        Type _approximation;

    };
    
    
}