//
//  TriggerParameterSwitchName.hpp
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
        
        class TriggerEditLayer;
        
        class TriggerParameterSwitchName : public TriggerParameterBase
        {
            
        public:
            
            TriggerParameterSwitchName() = default;
            explicit TriggerParameterSwitchName(GMXLayer* layer) : TriggerParameterBase(layer)
            {
                setParameterName("");
            }
            TriggerParameterSwitchName(const TriggerParameterSwitchName& rhs) : TriggerParameterBase(rhs) { copyFrom(rhs); }
            TriggerParameterSwitchName& operator=(const TriggerParameterSwitchName& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const TriggerParameterSwitchName& rhs)
            {
                TriggerParameterBase::copyFrom(rhs);
                _switchName = rhs._switchName;
                _currIndex = rhs._currIndex;
            }
            
            virtual ~TriggerParameterSwitchName()
            {
            }
            
            int getSwitchIndex() const { return _currIndex; }
            void setSwitchIndex(int index) { _currIndex = index; }
            
            std::string getSwitchName() const { return _switchName; }
            void setSwitchName(const std::string& name)
            {
                _switchName = name;
                setParameterName(_switchName);
            }
            
            virtual TriggerParameterSwitchName* clone() const override
            {
                return new TriggerParameterSwitchName(*this);
            }
            
            virtual void reset() override { _switchName.clear(); _currIndex = -1; }
            virtual bool isItemSelected() override { return (_currIndex != -1); }
            
            virtual void drawImGui(void* opt = nullptr) override
            {
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.85, 0.85, 0.85, 1.0));
                ImGui::PushItemWidth(600);
                std::string switchNames;
                auto triggerEditLayer = _gmxLayer->getTriggerEditLayer();
                const auto& switchNameList = triggerEditLayer->getSwitchNames();
                for( const auto& name : switchNameList )
                {
                    switchNames += name.data();
                    switchNames += '\0';
                }
                if ( ImGui::Combo("", &_currIndex, switchNames.c_str(), 16) )
                {
                    setSwitchName(switchNameList[_currIndex].data());
                }
                ImGui::PopItemWidth();
                ImGui::PopStyleColor();
            }
            
            virtual std::string getParameterName() const override
            {
                auto triggerEditLayer = _gmxLayer->getTriggerEditLayer();
                const auto& switchNameList = triggerEditLayer->getSwitchNames();
                if ( switchNameList[_currIndex].data() != _switchName )
                {
                    _switchName = switchNameList[_currIndex].data();
                    _parameterName = _switchName;
                }
                return _parameterName;
            }
            
        private:
            
            mutable std::string _switchName;
            int _currIndex = -1;
            
        };
        
    }
}









