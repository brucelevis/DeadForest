//
//  TriggerEditLayer.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 9..
//
//

#include "TriggerEditLayer.hpp"
#include "GMXLayer2.hpp"
#include "LocationNode.hpp"
using namespace cocos2d;
using namespace realtrick;


TriggerEditLayer::TriggerEditLayer(GMXLayer2& layer) :
_gmxLayer(layer)
{
}


TriggerEditLayer::~TriggerEditLayer()
{
}


TriggerEditLayer* TriggerEditLayer::create(GMXLayer2& layer)
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
            std::string name = "Player " + std::to_string(i+1);
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
        
        for( int i = 0 ; i < 5 ; ++ i)
        {
            ImGui::PushID(i);
            if (ImGui::Selectable("Conditions:\nAlways.\nActions:\nVictory game.\nPreserve Triggers.", &_selectedTrigger[i], ImGuiSelectableFlags_AllowDoubleClick))
            {
                for(int j = 0 ; j < 5 ; ++j )
                    _selectedTrigger[j] = false;
                _selectedTrigger[i] = true;
                
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
        ImGui::Button("New", ImVec2(130, 25));
        ImGui::Button("Modify", ImVec2(130, 25));
        ImGui::Button("Copy", ImVec2(130, 25));
        ImGui::Button("Delete", ImVec2(130, 25));
        ImGui::Button("Move Up", ImVec2(130, 25));
        ImGui::Button("Move Down", ImVec2(130, 25));
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









