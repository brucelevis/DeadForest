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
        
        ActionDisplayText() { _buf[0] = '\0'; }
        virtual ~ActionDisplayText() = default;
        
        ActionDisplayText(const ActionDisplayText& rhs) : ActionBase(rhs) { copyFrom(rhs); }
        ActionDisplayText& operator=(const ActionDisplayText& rhs)
        {
            if ( &rhs != this ) copyFrom(rhs);
            return *this;
        }
        
        void copyFrom(const ActionDisplayText& rhs)
        {
            std::strncpy(_buf, rhs._buf, 32);
        }

        virtual bool drawEditMode(void* opt) override
        {
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColor(ImVec4(0.85, 0.85, 0.85, 1.00)));
            ImGui::Text("Display for current player.");
            ImGui::InputText("", _buf, 32);
            ImGui::PopStyleColor();
            
            return (_buf[0] != '\0');
        }
        
        virtual bool drawSelectableSummary(bool& selected) const override
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
        
        virtual void reset() override
        {
            _buf[0] = '\0';
        }
        
        virtual ActionDisplayText* clone() const override
        {
            return new ActionDisplayText(*this);
        }
        
    private:
        
        char _buf[32];
        
    };
    
}









