//
//  SaveAsLayer.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 9..
//
//

#pragma once

#include <boost/filesystem.hpp>

#include "cocos2d.h"
#include "imgui.h"
#include "imgui_internal.h"

#include "EditScene.hpp"
#include "SizeProtocol.h"

namespace realtrick
{
    namespace editor
    {
        
        class EditScene;
        
        class SaveAsLayer : public cocos2d::Node
        {
            
        public:
            
            explicit SaveAsLayer(EditScene* layer) :
            _imguiLayer(layer),
            _filePath("")
            {
                strncpy(_filePath, cocos2d::UserDefault::getInstance()->getStringForKey("recent_save_path", "").c_str(), 256);
                checkIsSaveFile();
            }
            
            virtual ~SaveAsLayer() = default;
            static SaveAsLayer* create(EditScene* layer)
            {
                auto ret = new (std::nothrow) SaveAsLayer(layer);
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
                ImGui::OpenPopup("Save As");
                if (ImGui::BeginPopupModal("Save As", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    _imguiLayer->enableModal(true);
                    
                    if (ImGui::InputText("", _filePath, 256))
                    {
                        checkIsSaveFile();
                    }
                    
                    ImGui::TextColored(_isPossibleSave ? ImVec4(0,0.6,0,1) : ImVec4(0.6,0,0,1),
                                       _isPossibleSave ? "can" : "can not");
                    
                    auto style = ImGui::GetStyle();
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(style.Colors[ImGuiCol_Text].x,
                                                                style.Colors[ImGuiCol_Text].y,
                                                                style.Colors[ImGuiCol_Text].z,
                                                                _saveButtonTextAlpha));
                    
                    if (ImGui::ButtonEx("Save", ImVec2(60, 20), _saveButtonFlags))
                    {
                        _imguiLayer->saveFile(std::string(_filePath) + ".gmx");
                        cocos2d::UserDefault::getInstance()->setStringForKey("recent_save_path", _filePath);
                        cocos2d::UserDefault::getInstance()->flush();
                        opened = false;
                        closeWindow();
                    }
                    ImGui::PopStyleColor();
                    
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
            
            void checkIsSaveFile()
            {
				char slash = '/';
                std::string temp = _filePath;
                auto idx = temp.find_last_of(slash);
                temp = temp.substr(0, idx);
                temp += slash;
                
                boost::filesystem::path parentPath = boost::filesystem::path(temp);
                _isPossibleSave = (_filePath[0] != '\0' &&
                                   boost::filesystem::exists(parentPath) && boost::filesystem::is_directory(parentPath) && !boost::filesystem::exists(boost::filesystem::path(_filePath)));
                
                if ( _isPossibleSave )
                {
                    _saveButtonTextAlpha = 1.0f;
                    _saveButtonFlags = 0;
                }
                else
                {
                    _saveButtonTextAlpha = 0.5f;
                    _saveButtonFlags = ImGuiButtonFlags_Disabled;
                }
            }
            
        private:
            
            EditScene* _imguiLayer;
            
            char _filePath[256];
            ImGuiButtonFlags _saveButtonFlags = ImGuiButtonFlags_Disabled;
            float _saveButtonTextAlpha = 0.5f;
            bool _isPossibleSave = false;
            
        };
        
    }
}









