//
//  ConditionBase.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 18..
//
//

#pragma once

#include <vector>

#include "TriggerComponentProtocol.hpp"
#include "TriggerParameterBase.hpp"
#include "GMXLayer.hpp"

#include "imgui.h"
#include "imgui_internal.h"

namespace realtrick
{
    
    class ConditionBase : public TriggerComponentProtocol
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
        
        ConditionBase() = default;
        ConditionBase(const ConditionBase& rhs) = default;
        virtual ~ConditionBase() = default;
        
        virtual bool drawEditMode() override { return false; }
        virtual bool drawSelectableSummary(bool& selected) const override { return false; }
        virtual std::string getSummaryString() const override { return ""; }
        virtual void reset() override {}
        
    };
    
};









