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
#include "SizeProtocol.h"

namespace realtrick
{
    namespace editor
    {
        
        class ForceSettingLayer : public cocos2d::Node
        {
            
        public:
            
            explicit ForceSettingLayer(EditScene* layer) :
            _imguiLayer(layer)
            {
                _force1Name[0] = '\0';
                _force2Name[0] = '\0';
            }
            
            virtual ~ForceSettingLayer() = default;
            static ForceSettingLayer* create(EditScene* layer)
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
                if (ImGui::BeginPopupModal("Force Setting", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    _imguiLayer->enableModal(true);
                    
                    ImGui::BeginGroup();
                    ImGui::PushID(0);
                    ImGui::PushItemWidth(200);
                    ImGui::InputText("", _force1Name, 20);
                    ImGui::PopItemWidth();
                    ImGui::BeginChild("dummy1", ImVec2(200,200), true);
                    ImGui::EndChild();
                    ImGui::Checkbox("ally", &_isForce1Ally);
                    ImGui::SameLine(); ImGui::Checkbox("vision", &_isForce1Vision);
                    ImGui::PopID();
                    ImGui::EndGroup();
                    
                    ImGui::SameLine();
                    ImGui::BeginGroup();
                    ImGui::PushID(1);
                    ImGui::PushItemWidth(200);
                    ImGui::InputText("", _force2Name, 20);
                    ImGui::PopItemWidth();
                    ImGui::BeginChild("dummy2", ImVec2(200,200), true);
                    ImGui::EndChild();
                    ImGui::Checkbox("ally", &_isForce2Ally);
                    ImGui::SameLine(); ImGui::Checkbox("vision", &_isForce2Vision);
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
                _imguiLayer->enableModal(false);
            }
            
            
        private:
            
            EditScene* _imguiLayer;
            
            char _force1Name[20];
            bool _isForce1Ally = true;
            bool _isForce1Vision = true;
            
            char _force2Name[20];
            bool _isForce2Ally = true;
            bool _isForce2Vision = true;
            
        };
        
    }
}









