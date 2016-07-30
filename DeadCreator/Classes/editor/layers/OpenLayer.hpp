//
//  OpenLayer.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 10..
//
//

#pragma once

#include <boost/filesystem.hpp>

#include "cocos2d.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "EditScene.hpp"

namespace realtrick
{
    namespace editor
    {
        
        class EditScene;
        
        class OpenLayer : public cocos2d::Node
        {
            
        public:
            
            explicit OpenLayer(EditScene* layer) :
            _imguiLayer(layer),
            _filePath("")
            {
                strncpy(_filePath, cocos2d::UserDefault::getInstance()->getStringForKey("recent_open_path", "").c_str(), 256);
                checkIsOpenFile();
            }
            
            virtual ~OpenLayer() = default;
            static OpenLayer* create(EditScene* layer)
            {
                auto ret = new (std::nothrow) OpenLayer(layer);
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
                ImGui::OpenPopup("Open");
                if (ImGui::BeginPopupModal("Open", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    _imguiLayer->enableModal(true);
                    
                    if (ImGui::InputText("", _filePath, 256))
                    {
                        checkIsOpenFile();
                    }
                    
                    ImGui::TextColored(_isPossibleOpen ? ImVec4(0,0.6,0,1) : ImVec4(0.6,0,0,1),
                                       _isPossibleOpen ? "can" : "can not");
                    
                    auto style = ImGui::GetStyle();
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(style.Colors[ImGuiCol_Text].x,
                                                                style.Colors[ImGuiCol_Text].y,
                                                                style.Colors[ImGuiCol_Text].z,
                                                                _openButtonTextAlpha));
                    
                    if (ImGui::ButtonEx("Open", ImVec2(60, 20), _openButtonFlags))
                    {
                        _imguiLayer->createGMXLayer(_filePath);
                        
                        cocos2d::UserDefault::getInstance()->setStringForKey("recent_open_path", _filePath);
                        cocos2d::UserDefault::getInstance()->flush();
                        opened = false;
                        closeWindow();
                    }
                    ImGui::PopStyleColor();
                    
                    ImGui::SameLine();
                    if (ImGui::Button("Cancel", ImVec2(100, 20)))
                    {
                        closeWindow();
                        strncpy(_filePath, cocos2d::UserDefault::getInstance()->getStringForKey("recent_open_path", "").c_str(), 256);
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
            
            void checkIsOpenFile()
            {
                std::string temp = _filePath;
                std::string ext;
                if ( temp.size() > 4 )
                {
                    ext = temp.substr(temp.size() - 4, 4);
                }
                
                boost::filesystem::path leaf = boost::filesystem::path(temp).leaf().native();
                _isPossibleOpen = (boost::filesystem::exists(_filePath) && ext == ".gmx");
                
                if ( _isPossibleOpen )
                {
                    _openButtonTextAlpha = 1.0f;
                    _openButtonFlags = 0;
                }
                else
                {
                    _openButtonTextAlpha = 0.5f;
                    _openButtonFlags = ImGuiButtonFlags_Disabled;
                }
            }
            
        private:
            
            EditScene* _imguiLayer;
            
            char _filePath[256];
            ImGuiButtonFlags _openButtonFlags = ImGuiButtonFlags_Disabled;
            float _openButtonTextAlpha = 0.5f;
            bool _isPossibleOpen = false;
            
        };
        
    }
}









