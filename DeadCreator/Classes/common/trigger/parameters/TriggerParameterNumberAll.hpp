//
//  TriggerParameterNumberAll.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 4..
//
//

#pragma once

#include "TriggerParameterBase.hpp"
#include "StringHelper.hpp"

namespace realtrick
{
    namespace editor
    {
        
        class TriggerParameterNumberAll : public TriggerParameterBase
        {
            
        public:
            
            TriggerParameterNumberAll() : TriggerParameterBase(),
            _number(0)
            {
                setParameterName("all");
            }
            
            TriggerParameterNumberAll(const TriggerParameterNumberAll& rhs) { copyFrom(rhs); }
            TriggerParameterNumberAll& operator=(const TriggerParameterNumberAll& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const TriggerParameterNumberAll& rhs)
            {
                TriggerParameterBase::copyFrom(rhs);
                _isAll = rhs._isAll;
                _number = rhs._number;
                _label = rhs._label;
            }
            
            virtual ~TriggerParameterNumberAll() = default;
            
            std::pair<bool, int> getNumberAll() const { return { _isAll, _number }; }
            void setNumberAll(const std::pair<bool, int>& isAll_number)
            {
                _isAll = isAll_number.first;
                _number = isAll_number.second;
                
                if ( _isAll )
                {
                    setParameterName("all");
                    _label = "all";
                }
                else
                {
                    setParameterName(_to_string(_number));
                    _label = cocos2d::StringUtils::format("%3s", _to_string(_number).c_str());
                }
            }
            
            virtual TriggerParameterNumberAll* clone() const override
            {
                return new TriggerParameterNumberAll(*this);
            }
            
            virtual void reset() override { _number = 0; _isAll = true, _label = "all"; }
            virtual bool isItemSelected() override { return true; }
            
            virtual void drawImGui(void* opt = nullptr) override
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ComboBg]);
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyle().Colors[ImGuiCol_ComboBg]);
                ImGui::PushItemWidth(50);
                if ( ImGui::Button(_label.c_str()) )
                {
                    ImGui::OpenPopup("select");
                }
                if (ImGui::BeginPopup("select"))
                {
                    ImGui::PushItemWidth(30);
                    if ( ImGui::Checkbox("All", &_isAll) )
                    {
                        setNumberAll( { _isAll, _number } );
                    }
                    ImGui::PopItemWidth();
                    
                    if ( !_isAll )
                    {
                        ImGui::PushItemWidth(50);
                        ImGui::SameLine();
                        if ( ImGui::DragInt("", &_number, 1, 0, 255) )
                        {
                            if ( _number < 0 ) _number = 0;
                            if ( _number > 255 ) _number = 255;
                            
                            setNumberAll( { _isAll, _number } );
                        }
                        ImGui::PopItemWidth();
                    }
                    ImGui::EndPopup();
                }
                ImGui::PopItemWidth();
                ImGui::PopStyleColor(2);
            }
            
        private:
            
            int _number = 0;
            bool _isAll = true;
            std::string _label = "all";
            
        };
        
    }
}









