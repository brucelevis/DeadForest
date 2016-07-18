//
//  ConditionBase.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 18..
//
//

#pragma once

#include <vector>

#include "TriggerParameterBase.hpp"

#include "imgui.h"
#include "imgui_internal.h"

namespace realtrick
{
    
    class ConditionBase
    {
     
    public:
        
        enum class Type
        {
            INVALID = - 1,
            BRING = 0,
            ALWAYS,
            CONDITION_MAX
        };
        
    public:
        
        void pushParameter(TriggerParameterBase* parameter) { _parameters.push_back(parameter); }
        
        virtual void draw()  = 0;
        
    protected:
        
        std::vector<TriggerParameterBase*> _parameters;
        
    };
    
};