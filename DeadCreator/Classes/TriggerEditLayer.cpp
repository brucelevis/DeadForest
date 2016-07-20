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
    for( auto& cond : _conditionList)
        CC_SAFE_DELETE(cond);
    
    for( auto& act : _actionList)
        CC_SAFE_DELETE(act);
    
    
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
    
    _conditionList.resize(static_cast<int>(ConditionBase::Type::CONDITION_MAX));
    _conditionList[static_cast<int>(ConditionBase::Type::BRING)] = new ConditionBring(_gmxLayer);
    
    _actionList.resize(static_cast<int>(ActionBase::Type::ACTION_MAX));
    _actionList[static_cast<int>(ActionBase::Type::DISPLAY_TEXT)] = new ActionDisplayText(_gmxLayer);
    
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
        
        ImGui::BeginChild("dummy1", ImVec2(0, 150), true);
        
        for(int i = 0 ; i < 8 ; ++ i)
        {
            std::string name = "Player " + std::to_string(i + 1);
            if (ImGui::Selectable(name.c_str(), &_selectedPlayer[i]))
            {
                for(int j = 0 ; j < 8 ; ++j )
                    _selectedPlayer[j] = false;
                _selectedPlayer[i] = true;
            }
        }
        ImGui::EndChild();
        
        ImGui::BeginGroup();
        ImGui::BeginChild("dummy2", ImVec2(850, 480), true);
        
        for( int i = 0 ; i < _triggers.size() ; ++ i)
        {
            ImGui::PushID(i);
            if ( _triggers[i]->drawTrigger() )
            {
                for(int j = 0 ; j < _triggers.size() ; ++j )
                    _triggers[j]->setSelected(false);
                
                _triggers[i]->setSelected(true);
                
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
        
        ImGui::SameLine();
        ImGui::BeginGroup();
        ImGui::PushID(0);
        if ( ImGui::Button("New", ImVec2(130, 25)) )
        {
            ImGui::SetNextWindowPos(ImVec2((g.IO.DisplaySize.x - 1100) / 2,
                                           (g.IO.DisplaySize.x - 635) / 2),
                                    ImGuiSetCond_Once);
            ImGui::SetNextWindowSize(ImVec2(1100.0f, 645.0f));
            ImGui::OpenPopup("New Trigger");
            
            _newTrigger = new GameTrigger();
        }
        if (ImGui::BeginPopupModal("New Trigger", NULL, ImGuiWindowFlags_NoResize))
        {
            const char* tabNames[] = {"Player", "Conditions", "Actions"};
            const int numTabs = 3;
            const char* tabTooltips[numTabs] = {"", "", ""};
            static int selectedTab = 0;
            static int optionalHoveredTab = 0;
            ImGui::TabLabels(numTabs, tabNames, selectedTab, tabTooltips, false, &optionalHoveredTab);
            
            if ( selectedTab == 0 )
            {
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
                        _newTrigger->setPlayerType(checkedPlayerList);
                    }
                }
                
                ImGui::PopStyleColor();
                ImGui::EndChild();
            }
            
            else if ( selectedTab == 1 )
            {
                ImGui::BeginGroup();
                ImGui::BeginChild("##dummy", ImVec2(850, 550), true);
                
                _newTrigger->drawConditions();
                
                ImGui::EndChild();
                ImGui::EndGroup();
                
                ImGui::SameLine();
                ImGui::BeginGroup();
                ImGui::PushID(1);
                if ( ImGui::Button("New", ImVec2(130, 25)) )
                {
                    ImGui::SetNextWindowPos(ImVec2((g.IO.DisplaySize.x - 800) / 2,
                                                   (g.IO.DisplaySize.x - 345) / 2),
                                            ImGuiSetCond_Once);
                    ImGui::SetNextWindowSize(ImVec2(800, 345));
                    ImGui::OpenPopup("New Condition");
                }
                if ( ImGui::BeginPopupModal("New Condition", NULL, ImGuiWindowFlags_NoResize) )
                {
                    static int currentCondition = 0;
                    ImGui::Combo("Select Condition", &currentCondition, "Bring\0", 1);
                    ImGui::BeginChild("##dummy", ImVec2(0, 250), true);
                    
                    bool isCompleted = _conditionList[currentCondition]->drawEditMode();
                    
                    ImGui::EndChild();
                    
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
                        _newTrigger->addCondition(_conditionList[currentCondition]->clone());
                        _conditionList[currentCondition]->clear();
                        ImGui::CloseCurrentPopup();
                    }
                    
                    ImGui::GetStyle().Colors[ImGuiCol_Button] = backupButton;
                    ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = backupHoverColor;
                    ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = backupActiveColor;
                    
                    ImGui::SameLine();
                    if (ImGui::Button("Cancel", ImVec2(100, 20)))
                    {
                        _conditionList[currentCondition]->clear();
                        ImGui::CloseCurrentPopup();
                    }
                    
                    ImGui::EndPopup();
                }
                ImGui::Button("Modify", ImVec2(130, 25));
                ImGui::Button("Copy", ImVec2(130, 25));
                ImGui::Button("Delete", ImVec2(130, 25));
                ImGui::Button("Move Up", ImVec2(130, 25));
                ImGui::Button("Move Down", ImVec2(130, 25));
                ImGui::PopID();
                ImGui::EndGroup();
                
            }
            
            else if ( selectedTab == 2 )
            {
                
                ImGui::BeginGroup();
                ImGui::BeginChild("##dummy", ImVec2(850, 550), true);
                
                _newTrigger->drawActions();
                
                ImGui::EndChild();
                ImGui::EndGroup();
                
                ImGui::SameLine();
                ImGui::BeginGroup();
                ImGui::PushID(2);
                if ( ImGui::Button("New", ImVec2(130, 25)) )
                {
                    ImGui::SetNextWindowPos(ImVec2((g.IO.DisplaySize.x - 800) / 2,
                                                   (g.IO.DisplaySize.x - 345) / 2),
                                            ImGuiSetCond_Once);
                    ImGui::SetNextWindowSize(ImVec2(800, 345));
                    ImGui::OpenPopup("New Action");
                }
                if ( ImGui::BeginPopupModal("New Action", NULL, ImGuiWindowFlags_NoResize) )
                {
                    static int currentAction = 0;
                    ImGui::Combo("Select Action", &currentAction, "Display Text\0", 2);
                    ImGui::BeginChild("##dummy", ImVec2(0, 250), true);
                    
                    bool isCompleted = _actionList[currentAction]->drawEditMode();
                    
                    ImGui::EndChild();
                    
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
                        _newTrigger->addAction(_actionList[currentAction]->clone());
                        _actionList[currentAction]->clear();
                        ImGui::CloseCurrentPopup();
                    }
                    
                    ImGui::GetStyle().Colors[ImGuiCol_Button] = backupButton;
                    ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = backupHoverColor;
                    ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = backupActiveColor;

                    ImGui::SameLine();
                    if (ImGui::Button("Cancel", ImVec2(100, 20)))
                    {
                        ImGui::CloseCurrentPopup();
                    }
                    
                    ImGui::EndPopup();
                }
                
                ImGui::Button("Modify", ImVec2(130, 25));
                ImGui::Button("Copy", ImVec2(130, 25));
                ImGui::Button("Delete", ImVec2(130, 25));
                ImGui::Button("Move Up", ImVec2(130, 25));
                ImGui::Button("Move Down", ImVec2(130, 25));
                ImGui::PopID();
                ImGui::EndGroup();
                
            }
            
            bool isCompleted = (_newTrigger->getCheckedPlayerNumber() > 0 &&
                                _newTrigger->getConditionNumber() > 0 &&
                                _newTrigger->getActionNumber() > 0);
            
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
                _triggers.push_back(_newTrigger);
                
                for(int i = 0 ; i < 8 ; ++ i) _isPlayerChecked[i] = false;
                selectedTab = 0;
                
                ImGui::CloseCurrentPopup();
            }
            
            ImGui::GetStyle().Colors[ImGuiCol_Button] = backupButton;
            ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = backupHoverColor;
            ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = backupActiveColor;

            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(100, 20)))
            {
                // reset new trigger instance
                CC_SAFE_DELETE(_newTrigger);
                
                for(int i = 0 ; i < 8 ; ++ i) _isPlayerChecked[i] = false;
                selectedTab = 0;
                
                ImGui::CloseCurrentPopup();
            }
            
            ImGui::EndPopup();
        }
        
        ImGui::Button("Modify", ImVec2(130, 25));
        ImGui::Button("Copy", ImVec2(130, 25));
        ImGui::Button("Delete", ImVec2(130, 25));
        ImGui::Button("Move Up", ImVec2(130, 25));
        ImGui::Button("Move Down", ImVec2(130, 25));
        ImGui::PopID();
        ImGui::EndGroup();
        
        if(ImGui::Button("Ok", ImVec2(130, 25)))
        {
            *opened = false;
            closeWindow();
        }
        
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(130, 25)))
        {
            *opened = false;
            closeWindow();
        }
        
        ImGui::PopStyleVar();
        
        ImGui::EndPopup();
    }
}


void TriggerEditLayer::closeWindow()
{
    ImGui::CloseCurrentPopup();
}









