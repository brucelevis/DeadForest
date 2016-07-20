//
//  TriggerComponentProtocol.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 20..
//
//

#pragma once

#include <string>

#include "imgui.h"
#include "imgui_internal.h"

struct TriggerComponentProtocol
{
    virtual void drawEditMode()  = 0;
    virtual bool drawSelectableSummary() const = 0;
    virtual std::string getSummaryString() const = 0;
    virtual TriggerComponentProtocol* clone() const = 0;
};