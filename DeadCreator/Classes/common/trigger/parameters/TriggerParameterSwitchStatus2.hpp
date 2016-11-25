//
//  TriggerParameterSwitchStatus2.hpp
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
        
        class TriggerParameterSwitchStatus2 : public TriggerParameterBase
        {
            
        public:
            
            TriggerParameterSwitchStatus2() = default;
            explicit TriggerParameterSwitchStatus2(GMXLayer* layer) : TriggerParameterBase(layer)
            {
                setParameterName("");
            }
            
            TriggerParameterSwitchStatus2(const TriggerParameterSwitchStatus2& rhs) : TriggerParameterBase(rhs) { copyFrom(rhs); }
            TriggerParameterSwitchStatus2& operator=(const TriggerParameterSwitchStatus2& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const TriggerParameterSwitchStatus2& rhs)
            {
                TriggerParameterBase::copyFrom(rhs);
                _status = rhs._status;
                
                if ( _status == SwitchStatus2::CLEAR ) _currStatus = 0;
                else if ( _status == SwitchStatus2::SET ) _currStatus = 1;
                else if ( _status == SwitchStatus2::TOGGLE ) _currStatus = 2;
                else if ( _status == SwitchStatus2::RANDOMIZE ) _currStatus = 3;
            }
            
            virtual ~TriggerParameterSwitchStatus2() = default;
            
            SwitchStatus2 getSwitchStatus() const { return _status; }
            void setSwitchStatus(SwitchStatus2 status)
            {
                _status = status;
                
                if ( status == SwitchStatus2::CLEAR ) setParameterName("Clear");
                else if ( status == SwitchStatus2::SET ) setParameterName("Set");
                else if ( status == SwitchStatus2::TOGGLE ) setParameterName("Toggle");
                else if ( status == SwitchStatus2::RANDOMIZE ) setParameterName("Randomize");
                else setParameterName("##invalid");
            }
            
            virtual TriggerParameterSwitchStatus2* clone() const override
            {
                return new TriggerParameterSwitchStatus2(*this);
            }
            
            virtual void reset() override { _status = SwitchStatus2::INVALID; _currStatus = -1; }
            virtual bool isItemSelected() override { return (_currStatus != -1); }
            
            virtual void drawImGui(void* opt = nullptr) override
            {
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.85, 0.85, 0.85, 1.0));
                ImGui::PushItemWidth(200);
                if ( ImGui::Combo("", &_currStatus, "Clear\0Set\0Toggle\0Randomize\0", 4) )
                {
                    if ( _currStatus == 0 ) setSwitchStatus(SwitchStatus2::CLEAR);
                    else if ( _currStatus == 1 ) setSwitchStatus(SwitchStatus2::SET);
                    else if ( _currStatus == 2 ) setSwitchStatus(SwitchStatus2::TOGGLE);
                    else if ( _currStatus == 3 ) setSwitchStatus(SwitchStatus2::RANDOMIZE);
                    else setSwitchStatus(SwitchStatus2::INVALID);
                }
                ImGui::PopItemWidth();
                ImGui::PopStyleColor();
            }
            
        private:
            
            SwitchStatus2 _status;
            int _currStatus = -1;
            
        };
        
    }
}









