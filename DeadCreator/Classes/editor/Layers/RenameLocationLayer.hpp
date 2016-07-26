//
//  RenameLocationLayer.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 16..
//
//

#pragma once

#include "cocos2d.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "GMXLayer.hpp"
#include "LocationNode.hpp"

namespace realtrick
{
    
    class GMXLayer;
    
    class RenameLocationLayer : public cocos2d::Node
    {
        
    public:
        
        explicit RenameLocationLayer(GMXLayer& layer) : _gmxLayer(layer)
        {
            _locationName[0] = '\0';
        }
        
        virtual ~RenameLocationLayer() = default;
        static RenameLocationLayer* create(GMXLayer& layer)
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
        
        void showLayer(bool* opened)
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

        void closeWindow()
        {
            ImGui::CloseCurrentPopup();
            _gmxLayer.getGrabbedLocation()->setSelected(true);
        }
        
        void setInputText(const std::string& inputText) {  strncpy(_locationName, inputText.c_str(), 20); }
        
    private:
        
        GMXLayer& _gmxLayer;
        
        char _locationName[20];
        ImGuiButtonFlags _okButtonFlags = ImGuiButtonFlags_Disabled;
        float _okButtonTextAlpha = 0.5f;
        bool _isPossibleRenameLocation = false;
        
    };
    
}








