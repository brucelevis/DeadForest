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
        
        ActionBase(GMXLayer& layer) : _gmxLayer(layer) {}
        ActionBase(const ActionBase& rhs) : _gmxLayer(rhs._gmxLayer)
        {}
        
        virtual ~ActionBase() = default;
        
        virtual void drawEditMode() override {}
        virtual bool drawSelectableSummary() const override { return false; }
        virtual std::string getSummaryString() const override { return ""; }
        virtual ActionBase* clone() const override { return nullptr; }
        
    protected:
        
        GMXLayer& _gmxLayer;
        
    };
    
};