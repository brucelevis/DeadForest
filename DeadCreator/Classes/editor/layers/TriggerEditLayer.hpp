//
//  TriggerEditLayer.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 16..
//
//

#pragma once

#include <array>

#include "cocos2d.h"
#include "realtrick/imgui/imgui.h"
#include "realtrick/imgui/imgui_internal.h"

#include "GameTrigger.hpp"
#include "GMXFile_generated.h"
#include "Types.hpp"

namespace realtrick
{
    namespace editor
    {
        
        class GMXLayer;
        class ConditionBase;
        class ActionBase;
        
        class TriggerEditLayer : public cocos2d::Node
        {
            
        public:
            
            explicit TriggerEditLayer(GMXLayer& layer);
            virtual ~TriggerEditLayer();
            static TriggerEditLayer* create(GMXLayer& layer);
            virtual bool init() override;
            
            void showLayer(bool& opened);
            void closeWindow(bool& opened);
            
            void showTrigger(const char* title, bool& opened, GameTrigger* trigger, bool isModify = false, int index = -1);
            void showNewCondition(const char* title, bool& opened, GameTrigger* newTrigger);
            void showModifyCondition(const char* title, bool& opened, GameTrigger* trigger, int condIndex);
            void showNewAction(const char* title, bool& opened, GameTrigger* newTrigger);
            void showModifyAction(const char* title, bool& opened, GameTrigger* trigger, int actIndex);
            void showChangeSwitchName(const char* title, bool& opened);
            
            void saveTriggers(flatbuffers::FlatBufferBuilder& builder, std::vector<flatbuffers::Offset<DeadCreator::Trigger>>& out_triggers);
            void addTrigger(GameTrigger* trigger) { if ( trigger ) _triggers.push_back(trigger); }
            
            const std::vector<Switch>& getSwitchs() const { return _switchs; }
            void setSwitch(const Switch& swc) { CCASSERT((swc.index >= 0 && swc.index < 256), "out of index"); _switchs[swc.index] = swc; }
    
        private:
            
            GMXLayer& _gmxLayer;
            
            bool _isSelectedPlayer[8];
            int _selectedPlayer = 0;
            std::vector<GameTrigger*> _triggers;
            
            std::vector<ConditionBase*> _conditionList;
            std::vector<ActionBase*> _actionList;
            
            GameTrigger* _modifyingTrigger;
            ConditionBase* _modifyingCondition;
            ActionBase* _modifyingAction;
            
            std::vector<Switch> _switchs;
            std::array<char, 100> _tempSwitchNameForCompareOverlap;
            bool _isSelectedSwitchName[256];
            unsigned short _selectedSwitchNameIndex = 0;
            unsigned short _oldSelectedSwitchNameIndex = 0;
            
        };
        
    }
}









