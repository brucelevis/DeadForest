//
//  TriggerEditLayer.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 9..
//
//

#include "TriggerEditLayer.hpp"
#include "GMXLayer.hpp"
#include "LocationNode.hpp"
#include "TriggerParameterBase.hpp"
#include "TriggerParameters.hpp"
#include "GameTrigger.hpp"
#include "ConditionBase.hpp"
#include "ActionBase.hpp"
#include "Conditions.hpp"
#include "Actions.hpp"
#include "StringHelper.hpp"
using namespace cocos2d;
using namespace realtrick;
using namespace realtrick::editor;


TriggerEditLayer::TriggerEditLayer(GMXLayer& layer) :
_gmxLayer(layer)
{
}


TriggerEditLayer::~TriggerEditLayer()
{
    for(auto& trigger : _triggers) CC_SAFE_DELETE(trigger);
    for(auto& cond : _conditionList) CC_SAFE_DELETE(cond);
    for(auto& act : _actionList) CC_SAFE_DELETE(act);
    
    _triggers.clear();
    _conditionList.clear();
    _actionList.clear();
    
    _modifyingTrigger = nullptr;
    _modifyingCondition = nullptr;
    _modifyingAction = nullptr;
}


TriggerEditLayer* TriggerEditLayer::create(GMXLayer& layer)
{
    auto ret = new (std::nothrow) TriggerEditLayer(layer);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_FREE(ret);
    return nullptr;
}


bool TriggerEditLayer::init()
{
    if ( !Node::init() )
        return false;
    
    for ( int i = 0 ; i < 8 ; ++ i) _isSelectedPlayer[i] = false;
    _isSelectedPlayer[0] = true;
    _selectedPlayer = 0;
    
    _conditionList.push_back(new ConditionAlways(&_gmxLayer));
    _conditionList.push_back(new ConditionBring(&_gmxLayer));
    _conditionList.push_back(new ConditionCountdownTimer(&_gmxLayer));
    _conditionList.push_back(new ConditionElapsedTime(&_gmxLayer));
    _conditionList.push_back(new ConditionNever(&_gmxLayer));
    _conditionList.push_back(new ConditionSwitch(&_gmxLayer));
    
    _actionList.push_back(new ActionCreateEntity(&_gmxLayer));
    _actionList.push_back(new ActionDefeat(&_gmxLayer));
    _actionList.push_back(new ActionDisplayText(&_gmxLayer));
    _actionList.push_back(new ActionKillEntityAtLocation(&_gmxLayer));
    _actionList.push_back(new ActionMoveEntity(&_gmxLayer));
    _actionList.push_back(new ActionMoveLocation(&_gmxLayer));
    _actionList.push_back(new ActionOrder(&_gmxLayer));
    _actionList.push_back(new ActionPauseGame(&_gmxLayer));
    _actionList.push_back(new ActionPlaySound(&_gmxLayer));
    _actionList.push_back(new ActionPlaySoundAtLocation(&_gmxLayer));
    _actionList.push_back(new ActionPreserveTrigger(&_gmxLayer));
    _actionList.push_back(new ActionResumeGame(&_gmxLayer));
    _actionList.push_back(new ActionSetAllianceStatus(&_gmxLayer));
    _actionList.push_back(new ActionSetCountdownTimer(&_gmxLayer));
    _actionList.push_back(new ActionSetSwitch(&_gmxLayer));
    _actionList.push_back(new ActionVictory(&_gmxLayer));
    
    _switchs.resize(256);
    for(int i = 0 ; i < 256; ++ i)
    {
        std::string str = "Switch ";
        str += _to_string(i);
        std::strncpy(_switchs[i].name.data(), str.c_str(), 100);
        _switchs[i].status = SwitchStatus::CLEARED;
        _switchs[i].index = i;
        _isSelectedSwitchName[i] = false;
    }
    _selectedSwitchNameIndex = 0;
    _oldSelectedSwitchNameIndex = 0;
    _isSelectedSwitchName[_selectedSwitchNameIndex] = true;
    std::strncpy(_tempSwitchNameForCompareOverlap.data(), _switchs[_selectedSwitchNameIndex].name.data(), 100);
    
    return true;
}


void TriggerEditLayer::showLayer(bool& opened)
{
    ImGuiContext& g = *GImGui;
    ImGui::SetNextWindowPos(ImVec2((g.IO.DisplaySize.x - TRIGGER_EDIT_WIDTH) / 2,
                                   (g.IO.DisplaySize.y - TRIGGER_EDIT_HEIGHT) / 2), ImGuiSetCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(TRIGGER_EDIT_WIDTH, TRIGGER_EDIT_HEIGHT));
    ImGui::OpenPopup("Trigger Editor");
    if (ImGui::BeginPopupModal("Trigger Editor", NULL, ImGuiWindowFlags_NoResize))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
        
        static bool isShowNewTrigger = false;
        static bool isModifyTrigger = false;
        static bool isShowChangeSwitchName = false;
        
        //
        // draw player list
        //
        ImGui::BeginChild("dummy1", ImVec2(0, TRIGGER_PLAYER_HEIGHT), true);
        for(int i = 0 ; i < 8 ; ++ i)
        {
            std::string name = "Player " + _to_string(i + 1);
            if (ImGui::Selectable(name.c_str(), &_isSelectedPlayer[i]))
            {
                for(int j = 0 ; j < 8 ; ++j ) { _isSelectedPlayer[j] = false; }
                _isSelectedPlayer[i] = true;
                _selectedPlayer = i;
            }
        }
        ImGui::EndChild();
    
        //
        // draw trigger list
        //
        static int selectedTrigger = -1;
        ImGui::BeginGroup();
        ImGui::BeginChild("##dummy2", ImVec2(TRIGGER_EDIT_WIDTH - 150, TRIGGER_EDIT_HEIGHT - TRIGGER_PLAYER_HEIGHT - 70), true);
        for( int i = 0 ; i < _triggers.size() ; ++ i)
        {
            ImGui::PushID(i);
            if ( _triggers[i]->isPlayerSelected[_selectedPlayer] && _triggers[i]->drawSelectableTrigger() )
            {
                for(int j = 0 ; j < _triggers.size() ; ++j )
                    _triggers[j]->isSelected = false;
                
                _triggers[i]->isSelected = true;
                selectedTrigger = i;
                
                if ( ImGui::IsMouseDoubleClicked(0) )
                {
                    _modifyingTrigger = _triggers[selectedTrigger]->clone();
                    isModifyTrigger = true;
                }
            }
            ImGui::PopID();
        }
        ImGui::EndChild();
        ImGui::EndGroup();
        
        //
        // draw right buttons
        //
        ImGui::SameLine();
        ImGui::BeginGroup();
        ImGui::PushID(0);
        if ( ImGui::Button("New", ImVec2(130, 25)) )
        {
            _modifyingTrigger = new GameTrigger();
            isShowNewTrigger = true;
        }
        if ( ImGui::Button("Modify", ImVec2(130, 25)) )
        {
            if ( selectedTrigger != -1 )
            {
                _modifyingTrigger = _triggers[selectedTrigger]->clone();
                isModifyTrigger = true;
            }
        }
        if ( ImGui::Button("Copy", ImVec2(130, 25)) )
        {
            if ( selectedTrigger != -1)
            {
                auto clonedTrigger = _triggers[selectedTrigger]->clone();
                _triggers.push_back(clonedTrigger);
            }
        }
        if ( ImGui::Button("Delete", ImVec2(130, 25)) )
        {
            if ( selectedTrigger != -1)
            {
                auto iter = std::find(_triggers.begin(), _triggers.end(), _triggers[selectedTrigger]);
                if ( iter != _triggers.end() )
                {
                    CC_SAFE_DELETE(*iter);
                    _triggers.erase(iter);
                }
                if ( selectedTrigger == _triggers.size() ) selectedTrigger = -1;
                else _triggers[selectedTrigger]->isSelected = true;
            }
        }
        if ( ImGui::Button("Move Up", ImVec2(130, 25)) )
        {
            if ( selectedTrigger > 0 )
            {
                std::swap(_triggers[selectedTrigger], _triggers[selectedTrigger - 1]);
                selectedTrigger -= 1;
            }
        }
        if ( ImGui::Button("Move Down", ImVec2(130, 25)) )
        {
            if ( selectedTrigger < _triggers.size() - 1 )
            {
                std::swap(_triggers[selectedTrigger], _triggers[selectedTrigger + 1]);
                selectedTrigger += 1;
            }
        }
        if ( ImGui::Button("Switches", ImVec2(130, 25)) )
        {
            isShowChangeSwitchName = true;
        }
        ImGui::PopID();
        ImGui::EndGroup();
        
        //
        // draw bottom buttons
        //
        if(ImGui::Button("Ok", ImVec2(130, 25))) closeWindow(opened);
        ImGui::SameLine(); if (ImGui::Button("Cancel", ImVec2(130, 25))) closeWindow(opened);
        
        //
        // draw modal pop up windows
        //
        if ( isShowNewTrigger ) showTrigger("New Trigger", isShowNewTrigger, _modifyingTrigger);
        if ( isModifyTrigger ) showTrigger("Modify Trigger", isModifyTrigger, _modifyingTrigger, true, selectedTrigger);
        if ( isShowChangeSwitchName ) showChangeSwitchName("Change Swithch Name", isShowChangeSwitchName);
        
        ImGui::PopStyleVar();
        ImGui::EndPopup();
    }
}


void TriggerEditLayer::closeWindow(bool& opened)
{
    opened = false;
    
    for(int i = 0 ; i < 8 ; ++i ) _isSelectedPlayer[i] = false;
    _isSelectedPlayer[0] = true;
    
    for(int i = 0 ; i < _triggers.size() ; ++i ) _triggers[i]->isSelected = false;
    
    ImGui::CloseCurrentPopup();
}


void TriggerEditLayer::showTrigger(const char* title, bool& opened, GameTrigger* trigger, bool isModify, int index)
{
    static const int WIDTH = TRIGGER_EDIT_WIDTH + 100;
    static const int HEIGHT = TRIGGER_EDIT_HEIGHT - 55;
    ImGuiContext& g = *GImGui;
    ImGui::SetNextWindowPos(ImVec2((g.IO.DisplaySize.x - WIDTH) / 2, (g.IO.DisplaySize.y - HEIGHT) / 2), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT));
    ImGui::OpenPopup(title);
    
    if (ImGui::BeginPopupModal(title, NULL, ImGuiWindowFlags_NoResize))
    {
        const char* tabNames[] = {"Player", "Conditions", "Actions"};
        const int numTabs = 3;
        const char* tabTooltips[numTabs] = {"", "", ""};
        static int selectedTab = 0;
        static int optionalHoveredTab = 0;
        ImGui::TabLabels(numTabs, tabNames, selectedTab, tabTooltips, false, &optionalHoveredTab);
        
        if ( selectedTab == 0 )
        {
            //
            // draw select player layer
            //
            ImGui::BeginChild("##dummy", ImVec2(0, TRIGGER_EDIT_HEIGHT - 140), true);
            ImGui::Text("For which players will this trigger execute?");
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.85, 0.85, 0.85, 1.0));
            for(int i = 0 ; i < 8 ; ++ i)
            {
                std::string playerType = "Player " + _to_string(i + 1);
                ImGui::Checkbox(playerType.c_str(), &trigger->isPlayerSelected[i]);
            }
            ImGui::PopStyleColor();
            ImGui::EndChild();
        }
        
        else if ( selectedTab == 1 )
        {
            static bool isShowNewCondition = false;
            static bool isShowModifyCondition = false;
            
            //
            // draw conditions summary
            //
            static int selectedCondition = -1;
            ImGui::BeginGroup();
            ImGui::BeginChild("##dummy", ImVec2(TRIGGER_EDIT_WIDTH - 50, TRIGGER_EDIT_HEIGHT - 140), true);
            for (int i = 0 ; i < trigger->conditions.size(); ++ i)
            {
                ImGui::PushID(i);
                ImGui::Bullet();
                if ( trigger->conditions[i]->drawSelectableSummary(trigger->conditions[i]->isSelected()) )
                {
                    for(int j = 0 ; j < trigger->conditions.size(); ++ j)
                        trigger->conditions[j]->isSelected() = false;
                    
                    trigger->conditions[i]->isSelected() = true;
                    selectedCondition = i;
                    
                    if ( ImGui::IsMouseDoubleClicked(0) )
                    {
                        _modifyingCondition = trigger->conditions[selectedCondition]->clone();
                        isShowModifyCondition = true;
                    }
                }
                ImGui::PopID();
            }
            ImGui::EndChild();
            ImGui::EndGroup();
            
            if ( isShowNewCondition )
                showNewCondition("New Condition", isShowNewCondition, trigger);
            
            if ( isShowModifyCondition )
                showModifyCondition("Modify Condition", isShowModifyCondition, trigger, selectedCondition);
            
            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::PushID(1);
            if ( ImGui::Button("New", ImVec2(130, 25)) ) isShowNewCondition = true;
            if ( ImGui::Button("Modify", ImVec2(130, 25)) )
            {
                if ( selectedCondition != -1)
                {
                    _modifyingCondition = trigger->conditions[selectedCondition]->clone();
                    isShowModifyCondition = true;
                }
            }
            if ( ImGui::Button("Copy", ImVec2(130, 25)) )
            {
                if ( selectedCondition != -1)
                {
                    auto clonedCondition = trigger->conditions[selectedCondition]->clone();
                    clonedCondition->isSelected() = false;
                    trigger->addCondition(clonedCondition);
                }
            }
            if ( ImGui::Button("Delete", ImVec2(130, 25)) )
            {
                if ( selectedCondition != -1)
                {
                    trigger->eraseCondition(selectedCondition);
                    if ( selectedCondition == trigger->conditions.size() ) selectedCondition = -1;
                    else trigger->conditions[selectedCondition]->isSelected() = true;
                }
            }
            if ( ImGui::Button("Move Up", ImVec2(130, 25)) )
            {
                if ( selectedCondition > 0 )
                {
                    std::swap(trigger->conditions[selectedCondition], trigger->conditions[selectedCondition - 1]);
                    selectedCondition -= 1;
                }
            }
            if ( ImGui::Button("Move Down", ImVec2(130, 25)) )
            {
                if ( selectedCondition < trigger->conditions.size() - 1 )
                {
                    std::swap(trigger->conditions[selectedCondition], trigger->conditions[selectedCondition + 1]);
                    selectedCondition += 1;
                }
            }
            ImGui::PopID();
            ImGui::EndGroup();
        }
        
        else if ( selectedTab == 2 )
        {
            static bool isShowNewAction = false;
            static bool isShowModifyAction = false;
            
            //
            // draw actions summary
            //
            static int selectedAction = -1;
            ImGui::BeginGroup();
            ImGui::BeginChild("##dummy", ImVec2(TRIGGER_EDIT_WIDTH - 50, TRIGGER_EDIT_HEIGHT - 140), true);
            for (int i = 0 ; i < trigger->actions.size(); ++ i)
            {
                ImGui::PushID(i);
                ImGui::Bullet();
                if ( trigger->actions[i]->drawSelectableSummary(trigger->actions[i]->isSelected()) )
                {
                    for(int j = 0 ; j < trigger->actions.size(); ++ j)
                        trigger->actions[j]->isSelected() = false;
                    
                    trigger->actions[i]->isSelected() = true;
                    selectedAction = i;
                    
                    if ( ImGui::IsMouseDoubleClicked(0) )
                    {
                        _modifyingAction = trigger->actions[selectedAction]->clone();
                        isShowModifyAction = true;
                    }
                }
                ImGui::PopID();
            }
            ImGui::EndChild();
            ImGui::EndGroup();
            
            if ( isShowNewAction ) showNewAction("New Action", isShowNewAction, trigger);
            if ( isShowModifyAction ) showModifyAction("Modify Action", isShowModifyAction, trigger, selectedAction);
            
            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::PushID(2);
            if ( ImGui::Button("New", ImVec2(130, 25)) ) isShowNewAction = true;
            if ( ImGui::Button("Modify", ImVec2(130, 25)) )
            {
                if ( selectedAction != -1 )
                {
                    _modifyingAction = trigger->actions[selectedAction]->clone();
                    isShowModifyAction = true;
                }
            }
            if ( ImGui::Button("Copy", ImVec2(130, 25)) )
            {
                if ( selectedAction != -1 )
                {
                    auto clonedAction = trigger->actions[selectedAction]->clone();
                    clonedAction->isSelected() = false;
                    trigger->addAction(clonedAction);
                }
            }
            if ( ImGui::Button("Delete", ImVec2(130, 25)) )
            {
                if ( selectedAction != -1)
                {
                    trigger->eraseAction(selectedAction);
                    if ( selectedAction == trigger->actions.size() ) selectedAction = -1;
                    else trigger->actions[selectedAction]->isSelected() = true;
                }
            }
            if ( ImGui::Button("Move Up", ImVec2(130, 25)) )
            {
                if ( selectedAction > 0 )
                {
                    std::swap(trigger->actions[selectedAction], trigger->actions[selectedAction - 1]);
                    selectedAction -= 1;
                }
            }
            if ( ImGui::Button("Move Down", ImVec2(130, 25)) )
            {
                if ( selectedAction < trigger->actions.size() - 1 )
                {
                    std::swap(trigger->actions[selectedAction], trigger->actions[selectedAction + 1]);
                    selectedAction += 1;
                }
            }
            ImGui::PopID();
            ImGui::EndGroup();
            
        }
        
        bool isCompleted = (trigger->getSelectedPlayerSize() > 0 &&
                            trigger->conditions.size() > 0 &&
                            trigger->actions.size() > 0);
        
        ImVec4 backupButton = ImGui::GetStyle().Colors[ImGuiCol_Button];
        ImVec4 backupHoverColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
        ImVec4 backupActiveColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];
        
        if ( !isCompleted )
        {
            ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(0.20, 0.00, 0.00, 0.35);
            ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = ImVec4(0.20, 0.00, 0.00, 0.35);
            ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = ImVec4(0.20, 0.00, 0.00, 0.35);
        }
        
        if ( ImGui::Button("Ok", ImVec2(60, 20)) && isCompleted )
        {
            if ( isModify )
            {
                std::swap(_triggers[index], trigger);
                CC_SAFE_DELETE(trigger);
            }
            else addTrigger(trigger);
            
            // reset new trigger instance
            selectedTab = 0;
            
            opened = false;
            ImGui::CloseCurrentPopup();
        }
        
        ImGui::GetStyle().Colors[ImGuiCol_Button] = backupButton;
        ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = backupHoverColor;
        ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = backupActiveColor;
        
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(100, 20)))
        {
            // erase trigger instance
            CC_SAFE_DELETE(trigger);
            selectedTab = 0;
            
            opened = false;
            ImGui::CloseCurrentPopup();
        }
        
        ImGui::EndPopup();
    }
}


void TriggerEditLayer::showNewCondition(const char* title, bool& opened, GameTrigger* newTrigger)
{
    static const int WIDTH = TRIGGER_COMPONENT_EDIT_WIDTH;
    static const int HEIGHT = TRIGGER_COMPONENT_EDIT_HEIGHT;
    ImGuiContext& g = *GImGui;
    ImGui::OpenPopup(title);
    ImGui::SetNextWindowPos(ImVec2((g.IO.DisplaySize.x - WIDTH) / 2, (g.IO.DisplaySize.y - HEIGHT) / 2), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT));
    if ( ImGui::BeginPopupModal(title, NULL, ImGuiWindowFlags_NoResize) )
    {
        //
        // draw condition edit window
        //
        static int currentCondition = 0;
        static std::string conditionNameList;
        static bool isFirstCall = true;
        if ( isFirstCall )
        {
            for(int i = 0 ; i < _conditionList.size() ; ++ i)
            {
                conditionNameList += _conditionList[i]->name();
                conditionNameList += '\0';
            }
            isFirstCall = false;
        }
        ImGui::Combo("Select Condition", &currentCondition, conditionNameList.c_str(), 7);
        ImGui::BeginChild("##dummy", ImVec2(0, HEIGHT - 90), true);
        bool isCompleted = _conditionList[currentCondition]->drawEditMode(&_gmxLayer);
        ImGui::EndChild();
        
        //
        // draw ok button
        //
        ImVec4 backupButton = ImGui::GetStyle().Colors[ImGuiCol_Button];
        ImVec4 backupHoverColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
        ImVec4 backupActiveColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];
        if ( !isCompleted )
        {
            ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(0.20, 0.00, 0.00, 0.35);
            ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = ImVec4(0.20, 0.00, 0.00, 0.35);
            ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = ImVec4(0.20, 0.00, 0.00, 0.35);
        }
        if ( ImGui::ButtonEx("Ok", ImVec2(60, 20)) && isCompleted )
        {
            // save condition
            newTrigger->addCondition(_conditionList[currentCondition]->clone());
            
            // cleanup
            _conditionList[currentCondition]->reset();
            opened = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::GetStyle().Colors[ImGuiCol_Button] = backupButton;
        ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = backupHoverColor;
        ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = backupActiveColor;
        
        //
        // draw cancel button
        //
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(100, 20)))
        {
            // cleanup
            _conditionList[currentCondition]->reset();
            opened = false;
            ImGui::CloseCurrentPopup();
        }
        
        ImGui::EndPopup();
    }
}


void TriggerEditLayer::showModifyCondition(const char* title, bool& opened, GameTrigger* trigger, int condIndex)
{
    static const int WIDTH = TRIGGER_COMPONENT_EDIT_WIDTH;
    static const int HEIGHT = TRIGGER_COMPONENT_EDIT_HEIGHT;
    ImGuiContext& g = *GImGui;
    ImGui::OpenPopup(title);
    ImGui::SetNextWindowPos(ImVec2((g.IO.DisplaySize.x - WIDTH) / 2, (g.IO.DisplaySize.y - HEIGHT) / 2), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT));
    if ( ImGui::BeginPopupModal(title, NULL, ImGuiWindowFlags_NoResize) )
    {
        //
        // draw condition edit window
        //
        ImGui::Bullet(); ImGui::Text(_modifyingCondition->name().c_str(), NULL);
        ImGui::BeginChild("##dummy", ImVec2(0, HEIGHT - 90), true);
        bool isCompleted = _modifyingCondition->drawEditMode(&_gmxLayer);
        ImGui::EndChild();
        
        //
        // draw ok button
        //
        ImVec4 backupButton = ImGui::GetStyle().Colors[ImGuiCol_Button];
        ImVec4 backupHoverColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
        ImVec4 backupActiveColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];
        if ( !isCompleted )
        {
            ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(0.20, 0.00, 0.00, 0.35);
            ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = ImVec4(0.20, 0.00, 0.00, 0.35);
            ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = ImVec4(0.20, 0.00, 0.00, 0.35);
        }
        if ( ImGui::ButtonEx("Ok", ImVec2(60, 20)) && isCompleted )
        {
            // save
            std::swap(trigger->conditions[condIndex], _modifyingCondition);
            
            // revert
            CC_SAFE_DELETE(_modifyingCondition);
            
            // cleanup
            opened = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::GetStyle().Colors[ImGuiCol_Button] = backupButton;
        ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = backupHoverColor;
        ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = backupActiveColor;
        
        //
        // draw cancel button
        //
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(100, 20)))
        {
            // revert
            CC_SAFE_DELETE(_modifyingCondition);
            
            // cleanup
            opened = false;
            ImGui::CloseCurrentPopup();
        }
        
        ImGui::EndPopup();
    }
}


void TriggerEditLayer::showNewAction(const char* title, bool& opened, GameTrigger* newTrigger)
{
    static const int WIDTH = TRIGGER_COMPONENT_EDIT_WIDTH;
    static const int HEIGHT = TRIGGER_COMPONENT_EDIT_HEIGHT;
    ImGuiContext& g = *GImGui;
    ImGui::OpenPopup(title);
    ImGui::SetNextWindowPos(ImVec2((g.IO.DisplaySize.x - WIDTH) / 2, (g.IO.DisplaySize.y - HEIGHT) / 2), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT));
    if ( ImGui::BeginPopupModal(title, NULL, ImGuiWindowFlags_NoResize) )
    {
        //
        // draw action edit window
        //
        static int currentAction = 0;
        static std::string actionNameList;
        static bool isFirstCall = true;
        if ( isFirstCall )
        {
            for(int i = 0 ; i < _actionList.size() ; ++ i)
            {
                actionNameList += _actionList[i]->name();
                actionNameList += '\0';
            }
            isFirstCall = false;
        }
        ImGui::Combo("Select Action", &currentAction, actionNameList.c_str(), 7);
        ImGui::BeginChild("##dummy", ImVec2(0, HEIGHT - 90), true);
        bool isCompleted = _actionList[currentAction]->drawEditMode(&_gmxLayer);
        ImGui::EndChild();
        
        //
        // draw ok button
        //
        ImVec4 backupButton = ImGui::GetStyle().Colors[ImGuiCol_Button];
        ImVec4 backupHoverColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
        ImVec4 backupActiveColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];
        if ( !isCompleted )
        {
            ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(0.20, 0.00, 0.00, 0.35);
            ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = ImVec4(0.20, 0.00, 0.00, 0.35);
            ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = ImVec4(0.20, 0.00, 0.00, 0.35);
        }
        if ( ImGui::Button("Ok", ImVec2(60, 20)) && isCompleted )
        {
            // save
            newTrigger->addAction(_actionList[currentAction]->clone());
            
            // cleanup
            _actionList[currentAction]->reset();
            opened = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::GetStyle().Colors[ImGuiCol_Button] = backupButton;
        ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = backupHoverColor;
        ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = backupActiveColor;
        
        //
        // draw cancel button
        //
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(100, 20)))
        {
            // cleanup
            _actionList[currentAction]->reset();
            opened = false;
            ImGui::CloseCurrentPopup();
        }
        
        ImGui::EndPopup();
    }
}


void TriggerEditLayer::showModifyAction(const char* title, bool& opened, GameTrigger* trigger, int actIndex)
{
    static const int WIDTH = TRIGGER_COMPONENT_EDIT_WIDTH;
    static const int HEIGHT = TRIGGER_COMPONENT_EDIT_HEIGHT;
    ImGuiContext& g = *GImGui;
    ImGui::OpenPopup(title);
    ImGui::SetNextWindowPos(ImVec2((g.IO.DisplaySize.x - WIDTH) / 2, (g.IO.DisplaySize.y - HEIGHT) / 2), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT));
    if ( ImGui::BeginPopupModal(title, NULL, ImGuiWindowFlags_NoResize) )
    {
        //
        // draw condition edit window
        //
        ImGui::Bullet(); ImGui::Text(_modifyingAction->name().c_str(), NULL);
        ImGui::BeginChild("##dummy", ImVec2(0, HEIGHT - 90), true);
        bool isCompleted = _modifyingAction->drawEditMode(&_gmxLayer);
        ImGui::EndChild();
        
        //
        // draw ok button
        //
        ImVec4 backupButton = ImGui::GetStyle().Colors[ImGuiCol_Button];
        ImVec4 backupHoverColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
        ImVec4 backupActiveColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];
        if ( !isCompleted )
        {
            ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(0.20, 0.00, 0.00, 0.35);
            ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = ImVec4(0.20, 0.00, 0.00, 0.35);
            ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = ImVec4(0.20, 0.00, 0.00, 0.35);
        }
        if ( ImGui::ButtonEx("Ok", ImVec2(60, 20)) && isCompleted )
        {
            // save
            std::swap(trigger->actions[actIndex], _modifyingAction);
            
            // revert
            CC_SAFE_DELETE(_modifyingAction);
            
            // cleanup
            opened = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::GetStyle().Colors[ImGuiCol_Button] = backupButton;
        ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = backupHoverColor;
        ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = backupActiveColor;
        
        //
        // draw cancel button
        //
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(100, 20)))
        {
            // revert
            CC_SAFE_DELETE(_modifyingAction);
            
            // cleanup
            opened = false;
            ImGui::CloseCurrentPopup();
        }
        
        ImGui::EndPopup();
    }
}


void TriggerEditLayer::saveTriggers(flatbuffers::FlatBufferBuilder& builder, std::vector<flatbuffers::Offset<DeadCreator::Trigger>>& out_triggers)
{
    for (auto& trigger : _triggers)
    {
        std::vector<int> playerVector = trigger->getSelectedPlayers();
        std::vector<flatbuffers::Offset<DeadCreator::Condition>> conditionVector;
        std::vector<flatbuffers::Offset<DeadCreator::Action>> actionVector;
        for(auto& cond: trigger->conditions) conditionVector.push_back(cond->getConditionObject(builder));
        for(auto& act: trigger->actions) actionVector.push_back(act->getActionObject(builder));
        out_triggers.push_back(DeadCreator::CreateTrigger(builder,
                                                      builder.CreateVector(playerVector),
                                                      builder.CreateVector(conditionVector),
                                                      builder.CreateVector(actionVector)));
    }
}


void TriggerEditLayer::showChangeSwitchName(const char* title, bool& opened)
{
    static const int WIDTH = TRIGGER_COMPONENT_EDIT_WIDTH;
    static const int HEIGHT = TRIGGER_COMPONENT_EDIT_HEIGHT;
    ImGuiContext& g = *GImGui;
    ImGui::OpenPopup(title);
    ImGui::SetNextWindowPos(ImVec2((g.IO.DisplaySize.x - WIDTH) / 2, (g.IO.DisplaySize.y - HEIGHT) / 2), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT));
    ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(0.90, 0.90, 0.90, 0.90));
    if ( ImGui::BeginPopupModal(title, NULL, ImGuiWindowFlags_NoResize) )
    {
        ImGui::BeginChild("##dummy", ImVec2(0, HEIGHT - 65), true);
        for(int i = 0 ; i < 256 ; ++ i)
        {
            if ( ImGui::Selectable(_switchs[i].name.data(), _isSelectedSwitchName[i]) )
            {
                std::strncpy(_tempSwitchNameForCompareOverlap.data(), _switchs[i].name.data(), 100);
                
                _isSelectedSwitchName[_oldSelectedSwitchNameIndex] = false;
                _selectedSwitchNameIndex = i;
                _isSelectedSwitchName[_selectedSwitchNameIndex] = true;
                
                _oldSelectedSwitchNameIndex = i;
            }
        }
        ImGui::EndChild();
        
        static bool isOverlapName = false;
        ImGui::PushItemWidth(WIDTH - 470);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.85, 0.85, 0.85, 0.90));
        if ( ImGui::InputText("", _tempSwitchNameForCompareOverlap.data(), 100) )
        {
            isOverlapName = false;
            for(int i = 0 ; i < 256; ++ i)
            {
                if ( _selectedSwitchNameIndex == i ) continue;
                if ( strcmp(_tempSwitchNameForCompareOverlap.data(), _switchs[i].name.data()) == 0 )
                {
                    isOverlapName = true;
                    break;
                }
            }
        }
        ImGui::PopStyleColor();
        ImGui::PopItemWidth();
        
        ImGui::SameLine();
        
        ImVec4 backupButton = ImGui::GetStyle().Colors[ImGuiCol_Button];
        ImVec4 backupHoverColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
        ImVec4 backupActiveColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];
        if ( isOverlapName )
        {
            ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(0.20, 0.00, 0.00, 0.35);
            ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = ImVec4(0.20, 0.00, 0.00, 0.35);
            ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = ImVec4(0.20, 0.00, 0.00, 0.35);
        }
        if ( ImGui::ButtonEx("Set Name", ImVec2(145, 25)) && !isOverlapName )
        {
            std::strncpy(_switchs[_selectedSwitchNameIndex].name.data(), _tempSwitchNameForCompareOverlap.data(), 100);
        }
        ImGui::GetStyle().Colors[ImGuiCol_Button] = backupButton;
        ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = backupHoverColor;
        ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = backupActiveColor;
        
        ImGui::SameLine();
        if ( ImGui::ButtonEx("Reset Names", ImVec2(145, 25)) )
        {
            for(int i = 0 ; i < 256; ++ i)
            {
                std::string str = "Switch ";
                str += _to_string(i);
                std::strncpy(_switchs[i].name.data(), str.c_str(), 100);
                _isSelectedSwitchName[i] = false;
            }
            _isSelectedSwitchName[_selectedSwitchNameIndex] = true;
            std::strncpy(_tempSwitchNameForCompareOverlap.data(), _switchs[_selectedSwitchNameIndex].name.data(), 100);
        }
        
        // draw close button
        ImGui::SameLine();
        if (ImGui::Button("Close", ImVec2(145, 25)))
        {
            _isSelectedSwitchName[_selectedSwitchNameIndex] = false;
            _isSelectedSwitchName[0] = true;
            _oldSelectedSwitchNameIndex = _selectedSwitchNameIndex = 0;
            std::strncpy(_tempSwitchNameForCompareOverlap.data(), _switchs[_selectedSwitchNameIndex].name.data(), 100);
            
            opened = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
    ImGui::PopStyleColor();
}





