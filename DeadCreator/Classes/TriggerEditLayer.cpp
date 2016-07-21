//
//  TriggerEditLayer.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 9..
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
using namespace cocos2d;
using namespace realtrick;


TriggerEditLayer::TriggerEditLayer(GMXLayer& layer) :
_gmxLayer(layer)
{
}


TriggerEditLayer::~TriggerEditLayer()
{
    _conditionList.clear();
    _actionList.clear();
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
    
    for ( int i = 0 ; i < 8 ; ++ i)
    {
        _selectedPlayer[i] = false;
        _isPlayerChecked[i] = false;
    }
    _selectedPlayer[0] = true;
    
    GameTrigger::ConditionType condIter = GameTrigger::ConditionType::BEGIN;
    GameTrigger::increase(condIter);
    while ( condIter != GameTrigger::ConditionType::END )
    {
        _conditionList.push_back(GameTrigger::createCondition(condIter));
        GameTrigger::increase(condIter);
    }
    
    GameTrigger::ActionType actIter = GameTrigger::ActionType::BEGIN;
    GameTrigger::increase(actIter);
    while ( actIter != GameTrigger::ActionType::END )
    {
        _actionList.push_back(GameTrigger::createAction(actIter));
        GameTrigger::increase(actIter);
    }
    
    return true;
}

void TriggerEditLayer::showLayer(bool* opened)
{
    ImGuiContext& g = *GImGui;
    ImGui::SetNextWindowPos(ImVec2((g.IO.DisplaySize.x - 1000) / 2, (g.IO.DisplaySize.x - 700) / 2), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(1000, 700));
    ImGui::OpenPopup("Trigger Editor");
    if (ImGui::BeginPopupModal("Trigger Editor", NULL, ImGuiWindowFlags_NoResize))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
        
        static bool isShowNewTrigger = false;
        static bool isModifyTrigger = false;

        if ( isShowNewTrigger ) showNewTrigger("New Trigger", isShowNewTrigger);
        if ( isModifyTrigger ) { }
        
        //
        // draw player list
        //
        ImGui::BeginChild("dummy1", ImVec2(0, 150), true);
        for(int i = 0 ; i < 8 ; ++ i)
        {
            std::string name = "Player " + std::to_string(i + 1);
            if (ImGui::Selectable(name.c_str(), &_selectedPlayer[i]))
            {
                for(int j = 0 ; j < 8 ; ++j ) { _selectedPlayer[j] = false; }
                _selectedPlayer[i] = true;
            }
        }
        ImGui::EndChild();
    
        //
        // draw trigger list
        //
        ImGui::BeginGroup();
        ImGui::BeginChild("dummy2", ImVec2(850, 480), true);
        for( int i = 0 ; i < _triggers.size() ; ++ i)
        {
            ImGui::PushID(i);
            if ( _triggers[i].drawSelectableTrigger() )
            {
                for(int j = 0 ; j < _triggers.size() ; ++j )
                    _triggers[j].isSelected = false;
                
                _triggers[i].isSelected = true;
                
                if ( ImGui::IsMouseDoubleClicked(0) )
                {
                    log("modify mode %d", i);
                }
            }
            ImGui::PopID();
            ImGui::Separator();
        }
        ImGui::EndChild();
        ImGui::EndGroup();
        
        //
        // draw right buttons
        //
        ImGui::SameLine();
        ImGui::BeginGroup();
        ImGui::PushID(0);
        if ( ImGui::Button("New", ImVec2(130, 25)) ) isShowNewTrigger = true;
        if ( ImGui::Button("Modify", ImVec2(130, 25)) ) isModifyTrigger = true;
        ImGui::Button("Copy", ImVec2(130, 25));
        ImGui::Button("Delete", ImVec2(130, 25));
        ImGui::Button("Move Up", ImVec2(130, 25));
        ImGui::Button("Move Down", ImVec2(130, 25));
        ImGui::PopID();
        ImGui::EndGroup();
        
        //
        // draw bottom buttons
        //
        if(ImGui::Button("Ok", ImVec2(130, 25))) closeWindow(opened);
        ImGui::SameLine(); if (ImGui::Button("Cancel", ImVec2(130, 25))) closeWindow(opened);
        
        ImGui::PopStyleVar();
        ImGui::EndPopup();
    }
}


void TriggerEditLayer::closeWindow(bool* opened)
{
    *opened = false;
    
    for(int i = 0 ; i < 8 ; ++i )
        _selectedPlayer[i] = false;
    _selectedPlayer[0] = true;
    
    for(int i = 0 ; i < _triggers.size() ; ++i )
        _triggers[i].isSelected = false;
    
    ImGui::CloseCurrentPopup();
}


void TriggerEditLayer::showNewTrigger(const char* title, bool& opened)
{
    static GameTrigger newTrigger;
    
    ImGuiContext& g = *GImGui;
    ImGui::SetNextWindowPos(ImVec2((g.IO.DisplaySize.x - 1100) / 2, (g.IO.DisplaySize.x - 645) / 2), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(1100.0f, 645.0f));
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
            ImGui::BeginChild("##dummy", ImVec2(0, 550), true);
            ImGui::Text("For which players will this trigger execute?");
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.85, 0.85, 0.85, 1.0));
            for(int i = 0 ; i < 8 ; ++ i)
            {
                std::string playerType = "Player " + std::to_string(i + 1);
                if (ImGui::Checkbox(playerType.c_str(), &_isPlayerChecked[i]))
                {
                    std::vector<PlayerType> checkedPlayerList;
                    for(int i = 0 ; i < 8; ++ i)
                    {
                        if ( _isPlayerChecked[i] )
                        {
                            checkedPlayerList.push_back(static_cast<PlayerType>(i));
                        }
                    }
                    newTrigger.setPlayerType(checkedPlayerList);
                }
            }
            ImGui::PopStyleColor();
            ImGui::EndChild();
        }
        
        else if ( selectedTab == 1 )
        {
            //
            // draw conditions summary
            //
            static int selectedCondition = 0;
            ImGui::BeginGroup();
            ImGui::BeginChild("##dummy", ImVec2(950, 550), true);
            for (int i = 0 ; i < newTrigger.conditions.size(); ++ i)
            {
                bool ret = newTrigger.conditions[i]->drawSelectableSummary(newTrigger.conditions[i]->isSelected());
                if ( ret )
                {
                    for(int j = 0 ; j < newTrigger.conditions.size(); ++ j)
                        newTrigger.conditions[j]->isSelected() = false;
                    
                    newTrigger.conditions[i]->isSelected() = true;
                    selectedCondition = i;
                }
            }
            ImGui::EndChild();
            ImGui::EndGroup();
            
            static bool isShowNewCondition = false;
            static bool isShowModifyCondition = false;
            
            if ( isShowNewCondition )
                showNewCondition("New Condition", isShowNewCondition, newTrigger);
            
            if ( isShowModifyCondition )
                showModifyCondition("Modify Condition", isShowModifyCondition, newTrigger, selectedCondition);
            
            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::PushID(1);
            if ( ImGui::Button("New", ImVec2(130, 25)) ) isShowNewCondition = true;
            if ( ImGui::Button("Modify", ImVec2(130, 25)) )
            {
                _modifyingCondition = newTrigger.conditions[selectedCondition]->clone();
                isShowModifyCondition = true;
            }
            ImGui::Button("Copy", ImVec2(130, 25));
            ImGui::Button("Delete", ImVec2(130, 25));
            ImGui::Button("Move Up", ImVec2(130, 25));
            ImGui::Button("Move Down", ImVec2(130, 25));
            ImGui::PopID();
            ImGui::EndGroup();
        }
        
        else if ( selectedTab == 2 )
        {
            //
            // draw actions summary
            //
            static int selectedAction = 0;
            ImGui::BeginGroup();
            ImGui::BeginChild("##dummy", ImVec2(950, 550), true);
            for (int i = 0 ; i < newTrigger.actions.size(); ++ i)
            {
                bool ret = newTrigger.actions[i]->drawSelectableSummary(newTrigger.actions[i]->isSelected());
                if ( ret )
                {
                    for(int j = 0 ; j < newTrigger.actions.size(); ++ j)
                        newTrigger.actions[j]->isSelected() = false;
                    
                    newTrigger.actions[i]->isSelected() = true;
                    selectedAction = i;
                }
            }
            ImGui::EndChild();
            ImGui::EndGroup();
            
            static bool isShowNewAction = false;
            static bool isShowModifyAction = false;
            
            if ( isShowNewAction ) showNewAction("New Action", isShowNewAction, newTrigger);
            if ( isShowModifyAction ) showModifyAction("Modify Action", isShowModifyAction, newTrigger, selectedAction);
            
            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::PushID(2);
            if ( ImGui::Button("New", ImVec2(130, 25)) ) isShowNewAction = true;
            if ( ImGui::Button("Modify", ImVec2(130, 25)) )
            {
                _modifyingAction = newTrigger.actions[selectedAction]->clone();
                isShowModifyAction = true;
            }
            ImGui::Button("Copy", ImVec2(130, 25));
            ImGui::Button("Delete", ImVec2(130, 25));
            ImGui::Button("Move Up", ImVec2(130, 25));
            ImGui::Button("Move Down", ImVec2(130, 25));
            ImGui::PopID();
            ImGui::EndGroup();
            
        }
        
        bool isCompleted = (newTrigger.players.size() > 0 &&
                            newTrigger.conditions.size() > 0 &&
                            newTrigger.actions.size() > 0);
        
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
            // add trigger
            _triggers.push_back(newTrigger);
            
            // reset new trigger instance
            newTrigger.reset();
            for(int i = 0 ; i < 8 ; ++ i) _isPlayerChecked[i] = false;
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
            // reset new trigger instance
            newTrigger.reset();
            for(int i = 0 ; i < 8 ; ++ i) _isPlayerChecked[i] = false;
            selectedTab = 0;
            
            opened = false;
            ImGui::CloseCurrentPopup();
        }
        
        ImGui::EndPopup();
    }
}


void TriggerEditLayer::showNewCondition(const char* title, bool& opened, GameTrigger& newTrigger)
{
    ImGuiContext& g = *GImGui;
    ImGui::OpenPopup(title);
    ImGui::SetNextWindowPos(ImVec2((g.IO.DisplaySize.x - 800) / 2, (g.IO.DisplaySize.x - 345) / 2), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(800, 345));
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
        ImGui::Combo("Select Condition", &currentCondition, conditionNameList.c_str(), 3);
        bool isCompleted = _conditionList[currentCondition]->drawEditMode(&_gmxLayer);
    
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
            newTrigger.addCondition(_conditionList[currentCondition]->clone());
            
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


void TriggerEditLayer::showModifyCondition(const char* title, bool& opened, GameTrigger& trigger, int condIndex)
{
    ImGuiContext& g = *GImGui;
    ImGui::OpenPopup(title);
    ImGui::SetNextWindowPos(ImVec2((g.IO.DisplaySize.x - 800) / 2, (g.IO.DisplaySize.x - 345) / 2), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(800, 345));
    if ( ImGui::BeginPopupModal(title, NULL, ImGuiWindowFlags_NoResize) )
    {
        //
        // draw condition edit window
        //
        ImGui::Bullet(); ImGui::Text(_modifyingCondition->name().c_str(), NULL);
        bool isCompleted = _modifyingCondition->drawEditMode(&_gmxLayer);
        
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
            trigger.conditions[condIndex]->deepCopy(_modifyingCondition);
            
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


void TriggerEditLayer::showNewAction(const char* title, bool& opened, GameTrigger& newTrigger)
{
    ImGuiContext& g = *GImGui;
    ImGui::SetNextWindowPos(ImVec2((g.IO.DisplaySize.x - 800) / 2, (g.IO.DisplaySize.x - 345) / 2), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(800, 345));
    ImGui::OpenPopup(title);
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
        ImGui::Combo("Select Action", &currentAction, actionNameList.c_str(), 3);
        bool isCompleted = _actionList[currentAction]->drawEditMode(&_gmxLayer);
        
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
            newTrigger.addAction(_actionList[currentAction]->clone());
            
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


void TriggerEditLayer::showModifyAction(const char* title, bool& opened, GameTrigger& trigger, int actIndex)
{
    ImGuiContext& g = *GImGui;
    ImGui::OpenPopup(title);
    ImGui::SetNextWindowPos(ImVec2((g.IO.DisplaySize.x - 800) / 2, (g.IO.DisplaySize.x - 345) / 2), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(800, 345));
    if ( ImGui::BeginPopupModal(title, NULL, ImGuiWindowFlags_NoResize) )
    {
        //
        // draw condition edit window
        //
        ImGui::Bullet(); ImGui::Text(_modifyingAction->name().c_str(), NULL);
        bool isCompleted = _modifyingAction->drawEditMode(&_gmxLayer);
        
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
            trigger.actions[actIndex]->deepCopy(_modifyingAction);
            
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









