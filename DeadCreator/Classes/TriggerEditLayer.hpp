//
//  TriggerEditLayer.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 16..
//
//

#pragma once

#include "cocos2d.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace realtrick
{
    
    class GMXLayer;
    class GameTrigger;
    class ConditionBase;
    class ActionBase;
    
    class TriggerEditLayer : public cocos2d::Node
    {
        
    public:
        
        explicit TriggerEditLayer(GMXLayer& layer);
        virtual ~TriggerEditLayer();
        static TriggerEditLayer* create(GMXLayer& layer);
        virtual bool init() override;
        
        void showLayer(bool* opened);
        void closeWindow(bool* opened);
        
        void showNewTrigger(const char* title, bool& opened);
        void showNewCondition(const char* title, bool& opened, GameTrigger& newTrigger);
        void showModifyCondition(const char* title, bool& opened, GameTrigger& trigger, int condIndex);
        void showNewAction(const char* title, bool& opened, GameTrigger& newTrigger);
        
    private:
        
        GMXLayer& _gmxLayer;
        
        bool _selectedPlayer[8];
        bool _isPlayerChecked[8];
        
        std::vector<GameTrigger> _triggers;
        std::vector<ConditionBase*> _conditionList;
        std::vector<ActionBase*> _actionList;
        
        ConditionBase* _modifingCondition;
        
    };
    
}









