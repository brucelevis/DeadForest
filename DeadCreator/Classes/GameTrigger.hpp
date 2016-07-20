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
    
    class GameTrigger
    {
        
    public:
        
        bool drawTrigger()
        {
            std::string label;
            label += "Conditions:\n";
            for(int i = 0 ; i < _conditions.size(); ++ i)
            {
                label += _conditions[i]->getSummaryString();
                label += '\n';
            }
            
            label += "Actions:\n";
            for(int i = 0 ; i < _actions.size() ; ++ i)
            {
                label += _actions[i]->getSummaryString();
                label += '\n';
            }
            
            label.pop_back();
            
            bool ret = ImGui::Selectable(label.c_str(), &_isSelected, ImGuiSelectableFlags_AllowDoubleClick);
            return ret;
        }
        
        void setSelected(bool selected) { _isSelected = selected; }
        bool isSelected() const { return _isSelected; }
        
        void addPlayerType(PlayerType p) { _players.push_back(p); }
        void addCondition(ConditionBase* cond) { _conditions.push_back(cond); }
        void addAction(ActionBase* act) { _actions.push_back(act); }
        
        void drawConditions()
        {
            for(int i = 0 ; i < _conditions.size(); ++ i)
            {
                _conditions[i]->drawSelectableSummary();
            }
        }
        
        void drawActions()
        {
            for(int i = 0 ; i < _actions.size(); ++ i)
            {
                _actions[i]->drawSelectableSummary();
            }
        }
        
        
    private:
        
        bool _isSelected;
        
        std::vector<PlayerType> _players;
        std::vector<ConditionBase*> _conditions;
        std::vector<ActionBase*> _actions;
        
    };
    
}









