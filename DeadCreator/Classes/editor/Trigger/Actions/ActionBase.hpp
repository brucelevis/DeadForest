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
        
        ActionBase() = default;
        ActionBase(const ActionBase& rhs) : TriggerComponentProtocol(rhs) { copyFrom(rhs); }
        ActionBase& operator=(const ActionBase& rhs)
        {
            if ( &rhs != this) copyFrom(rhs);
            return *this;
        }
        virtual ~ActionBase() = default;
        void copyFrom(const ActionBase& rhs) { TriggerComponentProtocol::copyFrom(rhs); }
        
        virtual bool drawEditMode(void* opt) override { return false; }
        virtual std::string getSummaryString() const override { return ""; }
        virtual void reset() override {}
        virtual ActionBase* clone() const override { return nullptr; }
        virtual void deepCopy(TriggerComponentProtocol* copy) override { TriggerComponentProtocol::deepCopy(copy); }
        
    };
    
};









