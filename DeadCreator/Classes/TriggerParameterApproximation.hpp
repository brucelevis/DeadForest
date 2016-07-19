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
        
        TriggerParameterApproximation() : TriggerParameterBase(),
        _approximation(Type::INVALID)
        {
            _parameterType = TriggerParameterBase::Type::APPROXIMATION;
            setParameterName("#invalid");
        }
        
        Type getApproximationType() const { return _approximation; }
        void setApproximationType(Type type)
        {
            _approximation = type;
            
            if ( type == Type::AT_LEAST ) setParameterName("at least");
            else if ( type == Type::AT_MOST ) setParameterName("at most");
            else if ( type == Type::EXACTLY ) setParameterName("exactly");
        }
        
    private:
        
        Type _approximation;

    };
    
    
}