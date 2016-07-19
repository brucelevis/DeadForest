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
        _selectedPlayer[i] = false;
    _selectedPlayer[0] = true;
    
    for ( int i = 0 ; i < 5 ; ++ i)
        _selectedTrigger[i] = false;
    
    _conditionList.resize(static_cast<int>(ConditionBase::Type::CONDITION_MAX));
    _conditionList[static_cast<int>(ConditionBase::Type::BRING)] = new ConditionBring();
    
    _actionList.resize(static_cast<int>(ActionBase::Type::ACTION_MAX));
    _actionList[static_cast<int>(ActionBase::Type::DISPLAY_TEXT)] = new ActionDisplayText();
    
    return true;
}

void TriggerEditLayer::showLayer(bool* opened)
{
    ImGuiContext& g = *GImGui;
    ImGui::SetNextWindowPos(ImVec2((g.IO.DisplaySize.x - 800) / 2, (g.IO.DisplaySize.x - 500) / 2), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(800.0f, 500.0f));
    ImGui::OpenPopup("Trigger Editor");
    if (ImGui::BeginPopupModal("Trigger Editor", NULL, ImGuiWindowFlags_NoResize))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
        
        ImGui::BeginChild("dummy1", ImVec2(0, 100), true);
        
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
        ImGui::BeginChild("dummy2", ImVec2(650, 330), true);
        
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
        if (ImGui::Button("New", ImVec2(130, 25)))
        {
            ImGui::SetNextWindowPos(ImVec2((g.IO.DisplaySize.x - 1000) / 2,
                                           (g.IO.DisplaySize.x - 635) / 2),
                                    ImGuiSetCond_Once);
            ImGui::SetNextWindowSize(ImVec2(1000.0f, 645.0f));
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
            
            if ( selectedTab == 0)
            {
                ImGui::BeginChild("##dummy", ImVec2(0, 550), true);
                ImGui::Text("For which players will this trigger execute?");
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.85, 0.85, 0.85, 1.0));
                static bool isPlayerChecked[8] = { false, };
                for(int i = 0 ; i < 8 ; ++ i)
                {
                    std::string playerType = "Player " + std::to_string(i + 1);
                    ImGui::Checkbox(playerType.c_str(), &isPlayerChecked[i]);
                }
                ImGui::PopStyleColor();
                ImGui::EndChild();
            }
            
            else if ( selectedTab == 1 )
            {
                ImGui::BeginGroup();
                ImGui::BeginChild("##dummy", ImVec2(850, 550), true);
                ImGui::Text("Trigger Conditions");
                
                
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
                    
                    _conditionList[currentCondition]->draw();
                    
                    ImGui::EndChild();
                    
                    if (ImGui::Button("Ok", ImVec2(60, 20)))
                    {
                        // save
                        _newTrigger->addCondition(_conditionList[currentCondition]);
                        
                        ImGui::CloseCurrentPopup();
                    }
                    
                    ImGui::SameLine();
                    if (ImGui::Button("Cancel", ImVec2(100, 20)))
                    {
                        // reset
                        
                        
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
                ImGui::Text("Trigger Actions");
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
                    static int currentCondition = 0;
                    ImGui::Combo("Select Action", &currentCondition, "Display Text\0", 2);
                    ImGui::BeginChild("##dummy", ImVec2(0, 250), true);
                    
                    _actionList[currentCondition]->draw();
                    
                    ImGui::EndChild();
                    
                    if (ImGui::Button("Ok", ImVec2(60, 20)))
                    {
                        ImGui::CloseCurrentPopup();
                    }
                    
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
            
            if (ImGui::Button("Ok", ImVec2(60, 20)))
            {
                ImGui::CloseCurrentPopup();
            }
            
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









