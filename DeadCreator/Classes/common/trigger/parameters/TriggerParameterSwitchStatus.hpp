//
//  TriggerParameterSwitchStatus.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 26..
//
//

#pragma once

#include <array>

#include "TriggerParameterBase.hpp"
#include "TriggerEditLayer.hpp"


namespace realtrick
{
    namespace editor
    {
        
        class GMXLayer;
        
        class TriggerParameterSwitchStatus : public TriggerParameterBase
        {
            
        public:
            
            TriggerParameterSwitchStatus() = default;
            explicit TriggerParameterSwitchStatus(GMXLayer* layer) : TriggerParameterBase(layer)
            {
                setParameterName("");
            }
            
            TriggerParameterSwitchStatus(const TriggerParameterSwitchStatus& rhs) : TriggerParameterBase(rhs) { copyFrom(rhs); }
            TriggerParameterSwitchStatus& operator=(const TriggerParameterSwitchStatus& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const TriggerParameterSwitchStatus& rhs)
            {
                TriggerParameterBase::copyFrom(rhs);
                _status = rhs._status;
                if ( _status == SwitchStatus::CLEARED ) _currStatus = 0;
                else if ( _status == SwitchStatus::SET ) _currStatus = 1;
            }
            
            virtual ~TriggerParameterSwitchStatus() = default;
            
            SwitchStatus getSwitchStatus() const { return _status; }
            void setSwitchStatus(SwitchStatus status)
            {
                _status = status;
                
                if ( status == SwitchStatus::CLEARED ) setParameterName("Cleared");
                else if ( status == SwitchStatus::SET ) setParameterName("Set");
                else setParameterName("##invalid");
            }
            
            virtual TriggerParameterSwitchStatus* clone() const override
            {
                return new TriggerParameterSwitchStatus(*this);
            }
            
            virtual void reset() override { _status = SwitchStatus::INVALID; _currStatus = -1; }
            virtual bool isItemSelected() override { return (_currStatus != -1); }
            
            virtual void drawImGui(void* opt = nullptr) override
            {
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.85, 0.85, 0.85, 1.0));
                ImGui::PushItemWidth(200);
                if ( ImGui::Combo("", &_currStatus, "Cleared\0Set\0", 2) )
                {
                    if ( _currStatus == 0 ) setSwitchStatus(SwitchStatus::CLEARED);
                    else if ( _currStatus == 1 ) setSwitchStatus(SwitchStatus::SET);
                    else setSwitchStatus(SwitchStatus::INVALID);
                }
                ImGui::PopItemWidth();
                ImGui::PopStyleColor();
            }
            
        private:
            
            SwitchStatus _status;
            int _currStatus = -1;
            
        };
        
    }
}









