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
        ImGui::EndChild();
        
        ImGui::BeginChild("dummy2", ImVec2(650, 330), true);
        ImGui::EndChild();
        
        if(ImGui::Button("Ok", ImVec2(60, 23)))
        {
            *opened = false;
            closeWindow();
        }
        
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(100, 23)))
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









