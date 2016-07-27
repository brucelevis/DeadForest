//
//  TriggerParameterBase.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 18..
//
//

#pragma once

#include <string>

namespace realtrick
{
    namespace editor
    {
        
        class TriggerParameterBase
        {
            
        public:
            
            enum class Type
            {
                INVALID = -1,
                PLAYER = 0,
                ENTITY,
                LOCATION,
                APPROXIMATION,
                NUMBER,
                TEXT,
                SCORE,
                SWITCH,
                SWITCH_STATE,
                ORDER
            };
            
            TriggerParameterBase() :
            _parameterType(Type::INVALID),
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
                _parameterType = rhs._parameterType;
                _parameterName = rhs._parameterName;
            }
            
            virtual ~TriggerParameterBase() = default;
            
            Type getType() const { return _parameterType; }
            virtual std::string getParameterName() const { return _parameterName; }
            virtual void setParameterName(const std::string& name) { _parameterName = name; }
            
            virtual TriggerParameterBase* clone() const = 0;
            virtual void drawImGui(void* opt = nullptr) = 0;
            virtual void reset() = 0;
            virtual bool isItemSelected() = 0;
            
        protected:
            
            Type _parameterType;
            std::string _parameterName;
            
        };
        
    }
}









