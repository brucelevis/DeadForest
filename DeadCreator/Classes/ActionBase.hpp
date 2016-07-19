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

#include "imgui.h"
#include "imgui_internal.h"

namespace realtrick
{
    
    class ActionBase
    {
        
    public:
        
        enum class Type
        {
            INVALID = - 1,
            DISPLAY_TEXT = 0,
            ACTION_MAX
        };
        
    public:
        
        virtual void draw()  = 0;
        
    };
    
};