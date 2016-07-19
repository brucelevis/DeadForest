//
//  ActionDisplayText.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 19..
//
//

#pragma once

#include "ActionBase.hpp"
#include "TriggerParameters.hpp"

namespace realtrick
{
    
    class ActionDisplayText : public ActionBase
    {
        
    public:
        
        virtual void draw() override
        {
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColor(ImVec4(0.85, 0.85, 0.85, 1.00)));
            ImGui::Text("Display for current player.");
            ImGui::InputText("", _buf, 64);
            ImGui::PopStyleColor();
        }
        
    private:
        
        char _buf[64];
        
    };
    
}









