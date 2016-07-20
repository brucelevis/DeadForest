//
//  OpenLayer.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 9..
//
//

#include <boost/filesystem.hpp>
using namespace boost::filesystem;

#include "OpenLayer.hpp"
#include "EditScene.hpp"
#include "SizeProtocol.h"
using namespace cocos2d;
using namespace realtrick;

OpenLayer::OpenLayer(EditScene* layer) :
_imguiLayer(layer),
_filePath("")
{
    strncpy(_filePath, UserDefault::getInstance()->getStringForKey("recent_open_path", "").c_str(), 256);
    checkIsOpenFile();
}


OpenLayer::~OpenLayer()
{
}


OpenLayer* OpenLayer::create(EditScene* layer)
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


void OpenLayer::showLayer(bool* opened)
{
    ImGuiContext& g = *GImGui;
    ImGui::SetNextWindowPos(ImVec2((g.IO.DisplaySize.x - 430) / 2, (g.IO.DisplaySize.x - 200) / 2), ImGuiSetCond_Once);
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
            UserDefault::getInstance()->setStringForKey("recent_open_path", _filePath);
            UserDefault::getInstance()->flush();
            *opened = false;
            closeWindow();
        }
        ImGui::PopStyleColor();
        
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(100, 20)))
        {
            closeWindow();
            strncpy(_filePath, UserDefault::getInstance()->getStringForKey("recent_open_path", "").c_str(), 256);
            *opened = false;
        }
        ImGui::EndPopup();
    }
}


void OpenLayer::closeWindow()
{
    ImGui::CloseCurrentPopup();
    _imguiLayer->enableModal(false);
}


void OpenLayer::checkIsOpenFile()
{
    std::string temp = _filePath;
    std::string ext;
    if ( temp.size() > 4 )
    {
        ext = temp.substr(temp.size() - 4, 4);
    }
    
    path leaf = path(temp).leaf().native();
    _isPossibleOpen = (exists(_filePath) && ext == ".gmx");
    
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









