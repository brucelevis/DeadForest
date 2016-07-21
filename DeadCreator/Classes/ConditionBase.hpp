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

#include "imgui.h"
#include "imgui_internal.h"

namespace realtrick
{
    
    class ConditionBase : public TriggerComponentProtocol
    {
     
    public:
        
        ConditionBase() = default;
        ConditionBase(const ConditionBase& rhs) : TriggerComponentProtocol(rhs) { copyFrom(rhs); }
        ConditionBase& operator=(const ConditionBase& rhs)
        {
            if ( &rhs != this) copyFrom(rhs);
            return *this;
        }
        virtual ~ConditionBase() = default;
        void copyFrom(const ConditionBase& rhs) { TriggerComponentProtocol::copyFrom(rhs); }
        
        virtual bool drawEditMode(void* opt) override { return false; }
        virtual bool drawSelectableSummary(bool& selected) const override { return false; }
        virtual std::string getSummaryString() const override { return ""; }
        virtual void reset() override {}
        virtual ConditionBase* clone() const override { return nullptr; }
        virtual void deepCopy(TriggerComponentProtocol* copy) override { TriggerComponentProtocol::deepCopy(copy); }
    };
    
};









