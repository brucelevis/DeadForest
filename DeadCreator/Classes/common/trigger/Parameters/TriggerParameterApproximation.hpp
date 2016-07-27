//
//  TriggerParameterApproximation.hpp
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
        
        class TriggerParameterApproximation : public TriggerParameterBase
        {
            
        public:
            
            enum class Type
            {
                INVALID = -1,
                /* do not modify */
                AT_LEAST = 0,
                AT_MOST = 1,
                EXACTLY = 2,
                /* * * * * * * * */
            };
            
        public:
            
            TriggerParameterApproximation() :
            TriggerParameterBase(),
            _approximation(Type::INVALID)
            {
                setParameterName("#invalid");
            }
            
            TriggerParameterApproximation(const TriggerParameterApproximation& rhs) { copyFrom(rhs); }
            TriggerParameterApproximation& operator=(const TriggerParameterApproximation& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const TriggerParameterApproximation& rhs)
            {
                TriggerParameterBase::copyFrom(rhs);
                _approximation = rhs._approximation;
                _currApproximation = rhs._currApproximation;
            }
            
            virtual ~TriggerParameterApproximation() = default;
            
            Type getApproximationType() const { return _approximation; }
            void setApproximationType(Type type)
            {
                if ( type == Type::AT_LEAST ) setParameterName("at least");
                else if ( type == Type::AT_MOST ) setParameterName("at most");
                else if ( type == Type::EXACTLY ) setParameterName("exactly");
                
                
                _approximation = type;
                _currApproximation = static_cast<int>(type);
            }
            
            virtual TriggerParameterApproximation* clone() const override
            {
                return new TriggerParameterApproximation(*this);
            }
            
            virtual void reset() override { _currApproximation = -1; }
            virtual bool isItemSelected() override { return (_currApproximation != -1); }
            
            virtual void drawImGui(void* opt = nullptr) override
            {
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.85, 0.85, 0.85, 1.0));
                ImGui::PushItemWidth(150);
                static const char* items2[3] =
                {
                    "at least",
                    "at most",
                    "exactly",
                };
                if (ImGui::Combo("", &_currApproximation, items2, 3, 3))
                {
                    setApproximationType(static_cast<TriggerParameterApproximation::Type>(_currApproximation));
                }
                ImGui::PopItemWidth();
                ImGui::PopStyleColor();
            }
            
        private:
            
            Type _approximation;
            int _currApproximation = -1;
            
        };
        
    }
}









