//
//  GameTrigger.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 18..
//
//

#pragma once

#include <vector>
#include <string>

#include "EntityType.hpp"
#include "ConditionBase.hpp"
#include "Conditions.hpp"
#include "Actions.hpp"

#include "imgui.h"
#include "imgui_internal.h"

namespace realtrick
{
    
    class ConditionBase;
    class ActionBase;
    
    struct GameTrigger
    {
        
        bool isSelected;
        
        std::vector<PlayerType> players;
        std::vector<std::pair<bool, ConditionBase>> conditions;
        std::vector<ActionBase> actions;

        GameTrigger() = default;
        GameTrigger(const GameTrigger& rhs)
        {
            players = rhs.players;
        }
        
        void reset()
        {
            players.clear();
            conditions.clear();
            actions.clear();
        }
        
        bool drawTrigger()
        {
            std::string label;
            label += "Conditions:\n";
            for(int i = 0 ; i < conditions.size(); ++ i)
            {
                label += conditions[i].second.getSummaryString();
                label += '\n';
            }
            
            label += "Actions:\n";
            for(int i = 0 ; i < actions.size() ; ++ i)
            {
                label += actions[i].getSummaryString();
                label += '\n';
            }
            
            label.pop_back();
            
            bool ret = ImGui::Selectable(label.c_str(), &isSelected, ImGuiSelectableFlags_AllowDoubleClick);
            return ret;
        }
        
        void setPlayerType(const std::vector<PlayerType>& p) { players.clear(); players = p; }
        void addCondition(const ConditionBase& cond) { conditions.push_back({false, cond}); }
        void addAction(const ActionBase& act) { actions.push_back(act); }
        
    };
    
}









