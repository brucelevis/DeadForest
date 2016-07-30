//
//  TriggerParameterApproximation.hpp
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
        
        class TriggerParameterApproximation : public TriggerParameterBase
        {
            
        public:
            
            TriggerParameterApproximation() :
            TriggerParameterBase(),
            _approximation(ApproximationType::INVALID)
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
            
            ApproximationType getApproximationType() const { return _approximation; }
            void setApproximationType(ApproximationType type)
            {
                if ( type == ApproximationType::AT_LEAST ) setParameterName("at least");
                else if ( type == ApproximationType::AT_MOST ) setParameterName("at most");
                else if ( type == ApproximationType::EXACTLY ) setParameterName("exactly");
                
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
                    setApproximationType(static_cast<ApproximationType>(_currApproximation));
                }
                ImGui::PopItemWidth();
                ImGui::PopStyleColor();
            }
            
        private:
            
            ApproximationType _approximation;
            int _currApproximation = -1;
            
        };
        
    }
}









