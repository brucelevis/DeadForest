//
//  RenameLocationLayer.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 9..
//
//

#include "RenameLocationLayer.hpp"
#include "GMXLayer2.hpp"
#include "LocationNode.hpp"
using namespace cocos2d;
using namespace realtrick;

RenameLocationLayer::RenameLocationLayer(GMXLayer2& layer) :
_gmxLayer(layer)
{
    _locationName[0] = '\0';
}


RenameLocationLayer::~RenameLocationLayer()
{
}


RenameLocationLayer* RenameLocationLayer::create(GMXLayer2& layer)
{
    auto ret = new (std::nothrow) RenameLocationLayer(layer);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_FREE(ret);
    return nullptr;
}


void RenameLocationLayer::setInputText(const std::string& inputText)
{
    strncpy(_locationName, inputText.c_str(), 20);
}


void RenameLocationLayer::showLayer(bool* opened)
{
    ImGuiContext& g = *GImGui;
    ImGui::SetNextWindowPos(ImVec2((g.IO.DisplaySize.x - 430) / 2, (g.IO.DisplaySize.x - 200) / 2), ImGuiSetCond_Once);
    ImGui::OpenPopup("Change Location Name");
    if (ImGui::BeginPopupModal("Change Location Name", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if ( ImGui::InputText("", _locationName, 20) )
        {
            if ( _gmxLayer.isOverlappedLocationName(_locationName) )
            {
                _isPossibleRenameLocation = false;
                _okButtonTextAlpha = 0.5f;
                _okButtonFlags = ImGuiButtonFlags_Disabled;
            }
            else
            {
                _isPossibleRenameLocation = true;
                _okButtonTextAlpha = 1.0f;
                _okButtonFlags = 0;
            }
        }
        
        ImGui::TextColored(_isPossibleRenameLocation ? ImVec4(0, 0.6, 0, 1) : ImVec4(0.6, 0, 0, 1),
                           _isPossibleRenameLocation ? "can" : "can not");
        
        auto style = ImGui::GetStyle();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(style.Colors[ImGuiCol_Text].x,
                                                    style.Colors[ImGuiCol_Text].y,
                                                    style.Colors[ImGuiCol_Text].z,
                                                    _okButtonTextAlpha));
        
        if (ImGui::ButtonEx("Ok", ImVec2(60, 20), _okButtonFlags))
        {
            *opened = false;
            _gmxLayer.getGrabbedLocation()->setLocationName(_locationName);
            closeWindow();
        }
        ImGui::PopStyleColor();
        
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(100, 20)))
        {
            closeWindow();
            *opened = false;
        }
        
        ImGui::EndPopup();
    }
}


void RenameLocationLayer::closeWindow()
{
    ImGui::CloseCurrentPopup();
    _gmxLayer.getGrabbedLocation()->setSelected(true);
}




