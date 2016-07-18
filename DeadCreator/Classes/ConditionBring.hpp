//
//  ConditionBring.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 18..
//
//

#pragma once

#include "ConditionBase.hpp"
#include "TriggerParameters.hpp"

namespace realtrick
{
 
    class ConditionBring : public ConditionBase
    {
        
    public:
        
        virtual void draw() override
        {
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.85, 0.85, 0.85, 1.0));
            ImGui::PushID(0);
            ImGui::PushItemWidth(180);
            static const char* items1[9] = {
                "Player 1",
                "Player 2",
                "Player 3",
                "Player 4",
                "Player 5",
                "Player 6",
                "Player 7",
                "Player 8",
                "Current Player"
            };
            ImGui::Combo("", &_currPlayer, items1, 9, 9);
            ImGui::PopItemWidth();
            ImGui::PopID();
            
            ImGui::SameLine();
            ImGui::Text(" brings ");
            
            ImGui::SameLine();
            ImGui::PushID(1);
            ImGui::PushItemWidth(150);
            static const char* items2[3] = {
                "at least",
                "at most",
                "exactly",
            };
            ImGui::Combo("", &_currApproximation, items2, 3, 3);
            ImGui::PopItemWidth();
            ImGui::PopID();
            
            ImGui::SameLine();
            ImGui::PushID(2);
            ImGui::PushItemWidth(30);
            ImGui::DragInt("", &_currNumber, 1, 0, 255);
            ImGui::PopItemWidth();
            ImGui::PopID();
            
            ImGui::SameLine();
            ImGui::PushID(3);
            ImGui::PushItemWidth(150);
            static const char* items3[3] = {
                "Sheriff",
                "M16A2",
                "...",
            };
            ImGui::Combo("", &_currEntity, items3, 3, 3);
            ImGui::PopItemWidth();
            ImGui::PopID();
            
            ImGui::SameLine();
            ImGui::Text(" to ");
            
            ImGui::PushID(4);
            ImGui::PushItemWidth(150);
            static const char* items4[3] = {
                "Location 1",
                "Location 2",
                "Location 3",
            };
            ImGui::Combo("", &_currEntity, items4, 3, 3);
            ImGui::PopItemWidth();
            ImGui::PopID();
            
            
            ImGui::PopStyleColor();
        }
        
    private:
        
        int _currPlayer;
        TriggerParameterPlayerType* _playerType;
        
        int _currApproximation;
        TriggerParameterApproximation* _approximation;
        
        int _currNumber;
        TriggerParameterNumber* _number;
        
        int _currEntity;
        TriggerParameterEntity* _entity;
        
        int _currLocation;
        TriggerParameterLocation* _location;
        
        
        
    };
    
}