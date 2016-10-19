//
//  TriggerParameterArithmetical.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 19..
//
//

#pragma once

#include "TriggerParameterBase.hpp"

namespace realtrick
{
    namespace editor
    {
        
        class TriggerParameterArithmetical : public TriggerParameterBase
        {
            
        public:
            
            TriggerParameterArithmetical() :
            TriggerParameterBase(),
            _arithmetical(ArithmeticalType::INVALID)
            {
                setParameterName("#invalid");
            }
            
            TriggerParameterArithmetical(const TriggerParameterArithmetical& rhs) { copyFrom(rhs); }
            TriggerParameterArithmetical& operator=(const TriggerParameterArithmetical& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const TriggerParameterArithmetical& rhs)
            {
                TriggerParameterBase::copyFrom(rhs);
                _arithmetical = rhs._arithmetical;
                _currArithmetical = rhs._currArithmetical;
            }
            
            virtual ~TriggerParameterArithmetical() = default;
            
            ArithmeticalType getArithmeticalType() const { return _arithmetical; }
            void setArithmeticalType(ArithmeticalType type)
            {
                if ( type == ArithmeticalType::ADD ) setParameterName("add");
                else if ( type == ArithmeticalType::SET_TO ) setParameterName("set to");
                else if ( type == ArithmeticalType::SUBTRACT ) setParameterName("subtract");
            
                _arithmetical = type;
                _currArithmetical = static_cast<int>(type);
            }
            
            virtual TriggerParameterArithmetical* clone() const override
            {
                return new TriggerParameterArithmetical(*this);
            }
            
            virtual void reset() override { _currArithmetical = -1; }
            virtual bool isItemSelected() override { return (_currArithmetical != -1); }
            
            virtual void drawImGui(void* opt = nullptr) override
            {
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.85, 0.85, 0.85, 1.0));
                ImGui::PushItemWidth(150);
                static const char* items2[3] =
                {
                    "Add",
                    "Set to",
                    "Subtract",
                };
                ImGui::PushID(0);
                if (ImGui::Combo("", &_currArithmetical, items2, 3, 3))
                {
                    setArithmeticalType(static_cast<ArithmeticalType>(_currArithmetical));
                }
                ImGui::PopID();
                ImGui::PopItemWidth();
                ImGui::PopStyleColor();
            }
            
        private:
            
            ArithmeticalType _arithmetical;
            int _currArithmetical = -1;
            
        };
        
    }
}









