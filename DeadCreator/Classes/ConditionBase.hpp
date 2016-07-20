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
        
        ConditionBase(GMXLayer& layer) : _gmxLayer(layer) {}
        ConditionBase(const ConditionBase& rhs) : _gmxLayer(rhs._gmxLayer)
        {}
        
        virtual void drawEditMode() override {}
        virtual bool drawSelectableSummary() const override { return false; }
        virtual std::string getSummaryString() const override { return ""; }
        virtual ConditionBase* clone() const override { return nullptr; }
        
    protected:
        
        GMXLayer& _gmxLayer;
        
    };
    
};