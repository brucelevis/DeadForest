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
#include "imgui.h"
#include "imgui_internal.h"

#include "GameTrigger.hpp"
#include "GMXFile_generated.h"

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
            
            const std::vector<std::array<char, 100>>& getSwitchNames() const { return _switchNames; }
            void setSwitchName(int i, const std::array<char, 100>& name) { CCASSERT((i >= 0 && i < 256), "out of index"); _switchNames[i] = name; }
    
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
            
            std::vector<std::array<char, 100>> _switchNames;
            std::array<char, 100> _tempSwitchNameForCompareOverlap;
            bool _isSelectedSwitchName[256];
            unsigned short _selectedSwitchNameIndex = 0;
            unsigned short _oldSelectedSwitchNameIndex = 0;
            
        };
        
    }
}









