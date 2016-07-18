//
//  ActionBase.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 18..
//
//

#pragma once

#include <vector>

#include "TriggerParameterBase.hpp"

namespace realtrick
{
    
    class ActionBase
    {
        
    public:
        
        void pushParameter(TriggerParameterBase* parameter) { _parameters.push_back(parameter); }
        
    protected:
        
        std::vector<TriggerParameterBase*> _parameters;
        
    };
    
};