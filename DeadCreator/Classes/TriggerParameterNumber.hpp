//
//  TriggerParameterNumber.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 19..
//
//

#pragma once

#include "TriggerParameterBase.hpp"

namespace realtrick
{
    
    class TriggerParameterNumber : public TriggerParameterBase
    {
        
    public:
        
        TriggerParameterNumber() : TriggerParameterBase(),
        _number(0)
        {
            _parameterType = TriggerParameterBase::Type::NUMBER;
            setParameterName("0");
        }
        
        TriggerParameterNumber(const TriggerParameterNumber& rhs) { copyFrom(rhs); }
        TriggerParameterNumber& operator=(const TriggerParameterNumber& rhs)
        {
            if ( &rhs != this ) copyFrom(rhs);
            return *this;
        }
        
        void copyFrom(const TriggerParameterNumber& rhs)
        {
            TriggerParameterBase::copyFrom(rhs);
            _number = rhs._number;
        }
        
        virtual ~TriggerParameterNumber() = default;
        
        int getNumber() const { return _number; }
        void setNumber(int number)
        {
            _number = number;
            setParameterName(std::to_string(number));
        }
        
        virtual TriggerParameterNumber* clone() const override
        {
            return new TriggerParameterNumber(*this);
        }
        
    private:
        
        int _number;
        
    };
    
}









