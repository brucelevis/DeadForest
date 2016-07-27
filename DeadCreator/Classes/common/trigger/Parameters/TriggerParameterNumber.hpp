//
//  TriggerParameterNumber.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 19..
//
//

#pragma once

#include "TriggerParameterBase.hpp"

namespace realtrick
{
    namespace editor
    {
        
        class TriggerParameterNumber : public TriggerParameterBase
        {
            
        public:
            
            TriggerParameterNumber() : TriggerParameterBase(),
            _number(0)
            {
                _parameterType = TriggerParameterBase::Type::NUMBER;
                setParameterName("0");
            }
            
            TriggerParameterNumber(const TriggerParameterNumber& rhs) { copyFrom(rhs); }
            TriggerParameterNumber& operator=(const TriggerParameterNumber& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const TriggerParameterNumber& rhs)
            {
                TriggerParameterBase::copyFrom(rhs);
                _number = rhs._number;
            }
            
            virtual ~TriggerParameterNumber() = default;
            
            int getNumber() const { return _number; }
            void setNumber(int number)
            {
                _number = number;
                setParameterName(std::to_string(number));
            }
            
            virtual TriggerParameterNumber* clone() const override
            {
                return new TriggerParameterNumber(*this);
            }
            
            virtual void reset() override { _number = 0; }
            virtual bool isItemSelected() override { return true; }
            
            virtual void drawImGui(void* opt = nullptr) override
            {
                ImGui::PushItemWidth(30);
                if ( ImGui::DragInt("", &_number, 1, 0, 255) )
                {
                    if ( _number < 0 ) _number = 0;
                    if ( _number > 255 ) _number = 255;
                    
                    setParameterName(std::to_string(_number));
                }
                ImGui::PopItemWidth();
            }
            
        private:
            
            int _number = 0;
            
        };
        
    }
}









