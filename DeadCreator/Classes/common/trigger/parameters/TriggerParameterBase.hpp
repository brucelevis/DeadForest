//
//  TriggerParameterBase.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 18..
//
//

#pragma once

#include <string>

#include "EntityType.hpp"

#include "imgui.h"
#include "imgui_internal.h"

namespace realtrick
{
    namespace editor
    {
        
        class TriggerParameterBase
        {
            
        public:
            
            TriggerParameterBase() :
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
            
            std::string _parameterName;
            
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









