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
        
        ActionDisplayText(GMXLayer& layer) : ActionBase(layer)
        {
            _buf[0] = '\0';
        }
        
        virtual ~ActionDisplayText()
        {}
        
        ActionDisplayText(const ActionDisplayText& rhs) : ActionBase(rhs)
        {
            std::strncpy(_buf, rhs._buf, 32);
        }

        virtual bool drawEditMode() override
        {
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColor(ImVec4(0.85, 0.85, 0.85, 1.00)));
            ImGui::Text("Display for current player.");
            ImGui::InputText("", _buf, 32);
            ImGui::PopStyleColor();
            
            return (_buf[0] != '\0');
        }
        
        virtual bool drawSelectableSummary() const override
        {
            return ImGui::Selectable(this->getSummaryString().c_str());
        }
        
        virtual std::string getSummaryString() const override
        {
            std::string ret = "Display for current player. '";
            ret += _buf;
            ret += "'";
            return ret;
        }
        
        virtual ActionDisplayText* clone() const override
        {
            return new ActionDisplayText(*this);
        }
        
        virtual void clear() override
        {
            _buf[0] = '\0';
        }
        
    private:
        
        char _buf[32];
        
    };
    
}









