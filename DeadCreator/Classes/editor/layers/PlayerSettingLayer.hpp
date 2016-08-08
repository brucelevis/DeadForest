//
//  PlayerSettingLayer.hpp
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
        
        class PlayerSettingLayer : public cocos2d::Node
        {
            
        public:
            
            explicit PlayerSettingLayer(GMXLayer& layer) :
            _gmxLayer(layer)
            {
            }
            
            virtual ~PlayerSettingLayer() = default;
            static PlayerSettingLayer* create(GMXLayer& layer)
            {
                auto ret = new (std::nothrow) PlayerSettingLayer(layer);
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
                ImGui::OpenPopup("Player Setting");
                if (ImGui::BeginPopupModal("Player Setting", NULL))
                {
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
            
        };
        
    }
}









