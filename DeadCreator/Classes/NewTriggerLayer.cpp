//
//  NewTriggerLayer.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 9..
//
//

#include "NewTriggerLayer.hpp"
#include "GMXLayer2.hpp"
#include "LocationNode.hpp"
using namespace cocos2d;
using namespace realtrick;


NewTriggerLayer::NewTriggerLayer(GMXLayer2& layer) :
_gmxLayer(layer)
{
}


NewTriggerLayer::~NewTriggerLayer()
{
}


NewTriggerLayer* NewTriggerLayer::create(GMXLayer2& layer)
{
    auto ret = new (std::nothrow) NewTriggerLayer(layer);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_FREE(ret);
    return nullptr;
}


void NewTriggerLayer::showLayer(bool* opened)
{
    ImGuiContext& g = *GImGui;
    ImGui::SetNextWindowPos(ImVec2((g.IO.DisplaySize.x - 800) / 2, (g.IO.DisplaySize.x - 435) / 2), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(800.0f, 435.0f));
    ImGui::OpenPopup("Trigger Editor");
    if (ImGui::BeginPopupModal("Trigger Editor", NULL, ImGuiWindowFlags_NoResize))
    {
        const char* tabNames[] = {"Player", "Conditions", "Actions"};
        const int numTabs = 3;
        const char* tabTooltips[numTabs] = {"", "", ""};
        static int selectedTab = 0;
        static int optionalHoveredTab = 0;
        ImGui::TabLabels(numTabs, tabNames, selectedTab, tabTooltips, false, &optionalHoveredTab);
        
        ImGui::BeginChild("##dummy", ImVec2(0, 400 - 50), true);
        if ( selectedTab == 0)
        {
            ImGui::Text("Rendering");
        }
        else if ( selectedTab == 1)
        {
            ImGui::Text("Layers");
        }
        else if ( selectedTab == 2)
        {
            ImGui::Text("Actions");
        }
        ImGui::EndChild();
        
        ImGui::Button("Ok", ImVec2(60, 20));
        ImGui::SameLine(); ImGui::Button("Cancel", ImVec2(100, 20));
        
        ImGui::EndPopup();
    }
}


void NewTriggerLayer::closeWindow()
{
    ImGui::CloseCurrentPopup();
}









