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

        enum class ConditionType : int
        {
            BEGIN = 0,
            BRING,
            END,
        };
        
        enum class ActionType : int
        {
            BEGIN = 0,
            DISPLAY_TEXT,
            END,
        };
        
        struct ConditionData
        {
            ConditionType type;
            std::string name;
            
            ConditionData() = default;
            ConditionData(ConditionType t, const std::string& n) : type(t), name(n) {}
        };
        
        struct ActionData
        {
            ActionType type;
            std::string name;
            
            ActionData() = default;
            ActionData(ActionType t, const std::string& n) : type(t), name(n) {}
        };
        
        static void increase(ConditionType& type)
        {
            int temp = static_cast<int>(type);
            type = static_cast<GameTrigger::ConditionType>(++temp);
        }
        
        static void increase(ActionType& type)
        {
            int temp = static_cast<int>(type);
            type = static_cast<GameTrigger::ActionType>(++temp);
        }
        
        static ConditionBase* createCondition(ConditionType type)
        {
            switch (type)
            {
                case ConditionType::BRING: return new ConditionBring();
                default: { cocos2d::log("Invalid Condition Type"); return nullptr; }
            }
        }
        
        static ActionBase* createAction(ActionType type)
        {
            switch (type)
            {
                case ActionType::DISPLAY_TEXT: return new ActionDisplayText();
                default: { cocos2d::log("Invalid Action Type"); return nullptr; }
            }
        }
        
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
        void addCondition(ConditionBase* cond) { conditions.push_back(cond); }
        void addAction(ActionBase* act) { actions.push_back(act); }
        
    };
    
}









