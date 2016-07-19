//
//  SaveAsLayer.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 9..
//
//

#include <boost/filesystem.hpp>
using namespace boost::filesystem;

#include "SaveAsLayer.hpp"
#include "EditScene.hpp"
#include "SizeProtocol.h"
using namespace cocos2d;
using namespace realtrick;

SaveAsLayer::SaveAsLayer(EditScene* layer) :
_imguiLayer(layer),
_filePath("")
{
    strncpy(_filePath, UserDefault::getInstance()->getStringForKey("recent_save_path", "").c_str(), 256);
    checkIsSaveFile();
}


SaveAsLayer::~SaveAsLayer()
{
}


SaveAsLayer* SaveAsLayer::create(EditScene* layer)
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


void SaveAsLayer::showLayer(bool* opened)
{
    ImGuiContext& g = *GImGui;
    ImGui::SetNextWindowPos(ImVec2((g.IO.DisplaySize.x - 430) / 2, (g.IO.DisplaySize.x - 200) / 2), ImGuiSetCond_Once);
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
            UserDefault::getInstance()->setStringForKey("recent_save_path", _filePath);
            UserDefault::getInstance()->flush();
            *opened = false;
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


void SaveAsLayer::closeWindow()
{
    
    ImGui::CloseCurrentPopup();
    _imguiLayer->enableModal(false);
}


void SaveAsLayer::checkIsSaveFile()
{
    std::string temp = _filePath;
    auto idx = temp.find_last_of('/');
    temp = temp.substr(0, idx);
    temp += '/';
    
    path parentPath = path(temp);
    _isPossibleSave = (_filePath[0] != '\0' &&
                       exists(parentPath) && is_directory(parentPath) && !exists(path(_filePath)));
    
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






