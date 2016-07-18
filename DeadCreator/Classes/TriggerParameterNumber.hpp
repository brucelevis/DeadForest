//
//  TriggerParameterNumber.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 19..
//
//

#pragma once

namespace realtrick
{
    
    class TriggerParameterNumber : public TriggerParameterBase
    {
        
    public:
        
        explicit TriggerParameterNumber(int number) :
        TriggerParameterBase(),
        _number(number)
        {
            _parameterType = TriggerParameterBase::Type::NUMBER;
        }
        
        int getNumber() const { return _number; }
        void setNumber(int number) { _number = number; }
        
    private:
        
        int _number;
        
    };
    
}