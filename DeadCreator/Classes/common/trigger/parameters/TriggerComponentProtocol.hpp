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

namespace realtrick
{
    namespace editor
    {
        
        class TriggerComponentProtocol
        {
            
        public:
            
            TriggerComponentProtocol() = default;
            TriggerComponentProtocol(const TriggerComponentProtocol& rhs) { copyFrom(rhs); }
            TriggerComponentProtocol& operator=(const TriggerComponentProtocol& rhs)
            {
                if ( &rhs != this) copyFrom(rhs);
                return *this;
            }
            virtual ~TriggerComponentProtocol() = default;
            
            void copyFrom(const TriggerComponentProtocol& rhs)
            {
                _isSelected = rhs._isSelected;
                _name = rhs._name;
            }
            
            virtual void deepCopy(TriggerComponentProtocol* copy)
            {
                _isSelected = copy->_isSelected;
                _name = copy->_name;
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
            
            bool _isSelected = false;
            std::string _name;
            
        };
        
    }
}









