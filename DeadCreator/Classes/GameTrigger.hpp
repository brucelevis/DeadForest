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
        std::vector<ConditionBase*> conditions;
        std::vector<ActionBase*> actions;

        GameTrigger() = default;
        GameTrigger(const GameTrigger& rhs) { copyFrom(rhs); }
        GameTrigger& operator=(const GameTrigger& rhs)
        {
            if ( &rhs != this )
                copyFrom(rhs);
            return *this;
        }
        
        virtual ~GameTrigger() { reset(); }
        
        void copyFrom(const GameTrigger& rhs)
        {
            players = rhs.players;
            conditions = rhs.conditions;
            actions = rhs.actions;
        }
        
        void reset()
        {
            players.clear();
            conditions.clear();
            actions.clear();
        }
        
        bool drawSelectableTrigger()
        {
            std::string label;
            label += "Conditions:\n";
            for(int i = 0 ; i < conditions.size(); ++ i)
            {
                label += conditions[i]->getSummaryString();
                label += '\n';
            }
            
            label += "Actions:\n";
            for(int i = 0 ; i < actions.size() ; ++ i)
            {
                label += actions[i]->getSummaryString();
                label += '\n';
            }
            
            label.pop_back();
            
            bool ret = ImGui::Selectable(label.c_str(), &isSelected, ImGuiSelectableFlags_AllowDoubleClick);
            return ret;
        }
        
        void setPlayerType(const std::vector<PlayerType>& p) { players.clear(); players = p; }
        void addCondition(ConditionBase* cond) { cocos2d::log("addCondition"); conditions.push_back(cond); }
        void addAction(ActionBase* act) { actions.push_back(act); }
        
    };
    
}









