//
//  SaveQueryLayer.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 10..
//
//

#pragma once

#include "cocos2d.h"
#include "imguix/imgui.h"
#include "imguix/imgui_internal.h"

namespace realtrick
{
    namespace editor
    {
        
        class EditScene;
        
        class SaveQueryLayer : public cocos2d::Node
        {
            
        public:
            
            explicit SaveQueryLayer(EditScene* layer) : _imguiLayer(layer) {}
            virtual ~SaveQueryLayer() = default;
            static SaveQueryLayer* create(EditScene* layer)
            {
                auto ret = new (std::nothrow) SaveQueryLayer(layer);
                if ( ret && ret->init() )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            void showLayer(bool& opened)
            {
                ImGui::OpenPopup("Save Changes?");
                if (ImGui::BeginPopupModal("Save Changes?", &opened, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    _imguiLayer->enableModal(true);
                    if (ImGui::Button("Yes", ImVec2(60, 20)))
                    {
                        opened = false;
                        closeWindow();
                    }
                    
                    ImGui::SameLine();
                    if (ImGui::Button("No", ImVec2(60, 20)))
                    {
                        opened = false;
                        closeWindow();
                    }
                    
                    ImGui::SameLine();
                    if (ImGui::Button("Cancel", ImVec2(100, 20)))
                    {
                        opened = false;
                        closeWindow();
                    }
                    ImGui::EndPopup();
                }
            }
            
            void closeWindow()
            {
                _imguiLayer->enableModal(false);
                ImGui::CloseCurrentPopup();
            }
            
        private:
            
            EditScene* _imguiLayer;
            
        };
        
    }
}









