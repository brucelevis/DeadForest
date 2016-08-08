//
//  ForceSettingLayer.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 7..
//
//

#pragma once

#include <array>

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
            
            explicit ForceSettingLayer(GMXLayer& layer) :
            _gmxLayer(layer)
            {
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
            
            virtual bool init() override
            {
                if ( !Node::init() )
                    return false;
                
                return true;
            }
            
            void showLayer(bool& opened)
            {
                ImGui::OpenPopup("Force Setting");
                if (ImGui::BeginPopupModal("Force Setting", NULL,
                                           ImGuiWindowFlags_AlwaysAutoResize |
                                           ImGuiWindowFlags_NoResize |
                                           ImGuiWindowFlags_NoCollapse))
                {
                    auto& file = _gmxLayer.getFile();
                    
                    ImGui::BeginGroup();
                    ImGui::PushID(0);
                    ImGui::PushItemWidth(200);
                    ImGui::InputText("", file.force1.name.data(), 20);
                    ImGui::PopItemWidth();
                    ImGui::BeginChild("dummy1", ImVec2(200, 200), true);
                    
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
                    ImGui::Checkbox("ally", &file.force1.isAlly);
                    ImGui::SameLine(); ImGui::Checkbox("vision", &file.force1.isVision);
                    ImGui::PopStyleColor();
                    ImGui::PopID();
                    ImGui::EndGroup();
                    
                    ImGui::SameLine();
                    ImGui::BeginGroup();
                    ImGui::PushID(1);
                    ImGui::PushItemWidth(200);
                    ImGui::InputText("", file.force2.name.data(), 20);
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
                    ImGui::Checkbox("ally", &file.force2.isAlly);
                    ImGui::SameLine(); ImGui::Checkbox("vision", &file.force2.isVision);
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
            
        };
        
    }
}









