//
//  TriggerParameterAllyStatus.hpp
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
        
        class GMXLayer;
        
        class TriggerParameterAllyStatus : public TriggerParameterBase
        {
            
        public:
            
            TriggerParameterAllyStatus() = default;
            explicit TriggerParameterAllyStatus(GMXLayer* layer) :
            TriggerParameterBase(layer),
            _allyStatus(AllyStatusType::INVALID)
            {
                setParameterName("#invalid");
            }
            
            TriggerParameterAllyStatus(const TriggerParameterAllyStatus& rhs) : TriggerParameterBase(rhs) { copyFrom(rhs); }
            TriggerParameterAllyStatus& operator=(const TriggerParameterAllyStatus& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const TriggerParameterAllyStatus& rhs)
            {
                TriggerParameterBase::copyFrom(rhs);
                _allyStatus = rhs._allyStatus;
                _currAllyStatus = rhs._currAllyStatus;
            }
            
            virtual ~TriggerParameterAllyStatus() = default;
            
            AllyStatusType getAllyStatusType() const { return _allyStatus; }
            void setAllyStatusType(AllyStatusType type)
            {
                if ( type == AllyStatusType::ALLY ) setParameterName("ally");
                else if ( type == AllyStatusType::ENEMY ) setParameterName("enemy");
                
                _allyStatus = type;
                _currAllyStatus = static_cast<int>(type);
            }
            
            virtual TriggerParameterAllyStatus* clone() const override
            {
                return new TriggerParameterAllyStatus(*this);
            }
            
            virtual void reset() override { _currAllyStatus = -1; }
            virtual bool isItemSelected() override { return (_currAllyStatus != -1); }
            
            virtual void drawImGui(void* opt = nullptr) override
            {
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.85, 0.85, 0.85, 1.0));
                ImGui::PushItemWidth(150);
                static const char* items2[2] =
                {
                    "Ally",
                    "Enemy"
                };
                ImGui::PushID(0);
                if (ImGui::Combo("", &_currAllyStatus, items2, 2, 2))
                {
                    setAllyStatusType(static_cast<AllyStatusType>(_currAllyStatus));
                }
                ImGui::PopID();
                ImGui::PopItemWidth();
                ImGui::PopStyleColor();
            }
            
        private:
            
            AllyStatusType _allyStatus;
            int _currAllyStatus = -1;
            
        };
        
    }
}









