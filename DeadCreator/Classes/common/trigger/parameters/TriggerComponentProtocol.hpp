//
//  TriggerComponentProtocol.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 20..
//
//

#pragma once

#include <string>

#include "imgui.h"
#include "imgui_internal.h"

namespace realtrick
{
    namespace editor
    {
        
        class GMXLayer;
        
        class TriggerComponentProtocol
        {
            
        public:
            
            TriggerComponentProtocol() = default;
            explicit TriggerComponentProtocol(GMXLayer* gmxLayer) : _gmxLayer(gmxLayer) {}
            TriggerComponentProtocol(const TriggerComponentProtocol& rhs) { copyFrom(rhs); }
            TriggerComponentProtocol& operator=(const TriggerComponentProtocol& rhs)
            {
                if ( &rhs != this) copyFrom(rhs);
                return *this;
            }
            virtual ~TriggerComponentProtocol() = default;
            
            void copyFrom(const TriggerComponentProtocol& rhs)
            {
                _gmxLayer = rhs._gmxLayer;
                _isSelected = rhs._isSelected;
                _name = rhs._name;
            }
            
            bool& isSelected() { return _isSelected; }
            std::string& name() { return _name; }
            
            virtual bool drawSelectableSummary(bool& selected) const
            {
                return ImGui::Selectable(this->getSummaryString().c_str(), selected, ImGuiSelectableFlags_AllowDoubleClick);
            }
            
            virtual bool drawEditMode(void* opt)  = 0;
            virtual std::string getSummaryString() const = 0;
            virtual void reset() = 0;
            virtual TriggerComponentProtocol* clone() const = 0;
            
        protected:
            
            GMXLayer* _gmxLayer;
            
            bool _isSelected = false;
            std::string _name;
            
        };
        
    }
}









