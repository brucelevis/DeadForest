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
        
        TriggerParameterNumber() : TriggerParameterBase(),
        _number(-1)
        {
            _parameterType = TriggerParameterBase::Type::NUMBER;
            setParameterName("#invalid");
        }
        
        int getNumber() const { return _number; }
        void setNumber(int number)
        {
            _number = number;
            setParameterName(std::to_string(number));
        }
        
    private:
        
        int _number;
        
    };
    
}