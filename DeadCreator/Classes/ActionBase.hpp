//
//  ActionBase.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 18..
//
//

#pragma once

#include <vector>

#include "TriggerComponentProtocol.hpp"
#include "TriggerParameterBase.hpp"

#include "imgui.h"
#include "imgui_internal.h"

namespace realtrick
{
    
    class ActionBase : public TriggerComponentProtocol
    {
        
    public:
        
        enum class Type
        {
            INVALID = - 1,
            DISPLAY_TEXT = 0,
            ACTION_MAX
        };
        
    public:
        
        ActionBase() = default;
        ActionBase(const ActionBase& rhs) = default;
        virtual ~ActionBase() = default;
        
        virtual bool drawEditMode() override { return false; }
        virtual bool drawSelectableSummary(bool& selected) const override { return false; }
        virtual std::string getSummaryString() const override { return ""; }
        virtual void reset() override {}
        
    };
    
};









