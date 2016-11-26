//
//  TriggerParameterBase.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 18..
//
//

#pragma once

#include <string>

#include "Types.hpp"

#include "imguix/imgui.h"
#include "imguix/imgui_internal.h"

namespace realtrick
{
    namespace editor
    {
        
        class GMXLayer;
        
        class TriggerParameterBase
        {
            
        public:
        
            TriggerParameterBase() = default;
            explicit TriggerParameterBase(GMXLayer* gmxLayer) :
            _gmxLayer(gmxLayer),
            _parameterName("")
            {}
            
            TriggerParameterBase(const TriggerParameterBase& rhs) { copyFrom(rhs); }
            TriggerParameterBase& operator=(const TriggerParameterBase& rhs)
            {
                if ( &rhs != this) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const TriggerParameterBase& rhs)
            {
                _gmxLayer = rhs._gmxLayer;
                _parameterName = rhs._parameterName;
            }
            
            virtual ~TriggerParameterBase() = default;
            
            virtual std::string getParameterName() const { return _parameterName; }
            virtual void setParameterName(const std::string& name) { _parameterName = name; }
            
            virtual TriggerParameterBase* clone() const = 0;
            virtual void drawImGui(void* opt = nullptr) = 0;
            virtual void reset() = 0;
            virtual bool isItemSelected() = 0;
            
        protected:
            
            GMXLayer* _gmxLayer;
            mutable std::string _parameterName;
            
        };
        
    }
    
    namespace client
    {
        
        struct TriggerDataBase
        {
            virtual ~TriggerDataBase() = default;
            TriggerComponentType type;
        };
        
    }
}









