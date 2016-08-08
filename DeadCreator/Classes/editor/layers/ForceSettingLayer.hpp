//
//  ForceSettingLayer.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 7..
//
//

#pragma once

#include "cocos2d.h"
#include "imgui.h"
#include "imgui_internal.h"

#include "EditScene.hpp"
#include "GMXLayer.hpp"
#include "SizeProtocol.h"

namespace realtrick
{
    namespace editor
    {
        
        class ForceSettingLayer : public cocos2d::Node
        {
            
        public:
            
            ForceSettingLayer(GMXLayer& layer) :
            _gmxLayer(layer)
            {
                std::strncpy(_force1Name, "Force 1", 20);
                std::strncpy(_force2Name, "Force 2", 20);
            }
            
            virtual ~ForceSettingLayer() = default;
            static ForceSettingLayer* create(GMXLayer& layer)
            {
                auto ret = new (std::nothrow) ForceSettingLayer(layer);
                if ( ret && ret->init() )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_FREE(ret);
                return nullptr;
            }
            
            void showLayer(bool& opened)
            {
                ImGui::OpenPopup("Force Setting");
                if (ImGui::BeginPopupModal("Force Setting", NULL,
                                           ImGuiWindowFlags_AlwaysAutoResize |
                                           ImGuiWindowFlags_NoResize |
                                           ImGuiWindowFlags_NoCollapse))
                {
                    ImGui::BeginGroup();
                    ImGui::PushID(0);
                    ImGui::PushItemWidth(200);
                    ImGui::InputText("", _force1Name, 20);
                    ImGui::PopItemWidth();
                    ImGui::BeginChild("dummy1", ImVec2(200,200), true);
                    
                    if ( _isItemClicked && ImGui::IsMouseReleased(0) && ImGui::IsMouseHoveringWindow() )
                    {
                        _gmxLayer.getPlayerInfos().at(_clickedPlayer - 1).force = Force::FORCE_1;
                        _isItemClicked = false;
                    }
                    
                    for( const auto& player : _gmxLayer.getPlayerInfos() )
                    {
                        const PlayerInfo& info = player;
                        if ( info.force != Force::FORCE_1 || info.owner != Owner::HUMAN ) continue;
                        std::string name = "Player " + _to_string(static_cast<int>(info.player));
                        ImGui::Selectable(name.c_str());
                        if ( ImGui::IsItemClicked() )
                        {
                            _isItemClicked = true;
                            _clickedPlayer = static_cast<int>(info.player);
                        }
                    }
                    
                    ImGui::EndChild();
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyle().Colors[ImGuiCol_ComboBg]);
                    ImGui::Checkbox("ally", &_isForce1Ally);
                    ImGui::SameLine(); ImGui::Checkbox("vision", &_isForce1Vision);
                    ImGui::PopStyleColor();
                    ImGui::PopID();
                    ImGui::EndGroup();
                    
                    ImGui::SameLine();
                    ImGui::BeginGroup();
                    ImGui::PushID(1);
                    ImGui::PushItemWidth(200);
                    ImGui::InputText("", _force2Name, 20);
                    ImGui::PopItemWidth();
                    ImGui::BeginChild("dummy2", ImVec2(200,200), true);
                    
                    if ( _isItemClicked && ImGui::IsMouseReleased(0) && ImGui::IsMouseHoveringWindow() )
                    {
                        _gmxLayer.getPlayerInfos().at(_clickedPlayer - 1).force = Force::FORCE_2;
                        _isItemClicked = false;
                    }
                    
                    for( const auto& player : _gmxLayer.getPlayerInfos() )
                    {
                        const PlayerInfo& info = player;
                        if ( info.force != Force::FORCE_2 || info.owner != Owner::HUMAN ) continue;
                        std::string name = "Player " + _to_string(static_cast<int>(info.player));
                        ImGui::Selectable(name.c_str());
                        if ( ImGui::IsItemClicked() )
                        {
                            _isItemClicked = true;
                            _clickedPlayer = static_cast<int>(info.player);
                        }
                    }

                    ImGui::EndChild();
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyle().Colors[ImGuiCol_ComboBg]);
                    ImGui::Checkbox("ally", &_isForce2Ally);
                    ImGui::SameLine(); ImGui::Checkbox("vision", &_isForce2Vision);
                    ImGui::PopStyleColor();
                    ImGui::PopID();
                    ImGui::EndGroup();
                    
                    if ( ImGui::Button("Ok", ImVec2(100, 20)) )
                    {
                        closeWindow();
                        opened = false;
                    }
                    
                    ImGui::SameLine();
                    if (ImGui::Button("Cancel", ImVec2(100, 20)))
                    {
                        closeWindow();
                        opened = false;
                    }
                    ImGui::EndPopup();
                }
                
            }
            
            void closeWindow()
            {
                ImGui::CloseCurrentPopup();
            }
            
        private:
            
            GMXLayer& _gmxLayer;
            
            bool _isItemClicked = false;
            int _clickedPlayer = 1;
            
            char _force1Name[20];
            bool _isForce1Ally = true;
            bool _isForce1Vision = true;
            
            char _force2Name[20];
            bool _isForce2Ally = true;
            bool _isForce2Vision = true;
            
        };
        
    }
}









