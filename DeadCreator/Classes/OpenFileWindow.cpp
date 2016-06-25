//
//  OpenFileWindow.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 24..
//
//

#include <functional>

#include "OpenFileWindow.hpp"
#include "FileSystem.hpp"
#include "EditScene.h"
#include "SizeProtocol.h"
using namespace cocos2d;

OpenFileWindow::OpenFileWindow(EditScene* layer) :
_imguiLayer(layer)
{
}


OpenFileWindow::~OpenFileWindow()
{
}


OpenFileWindow* OpenFileWindow::create(EditScene* layer)
{
    auto ret = new (std::nothrow) OpenFileWindow(layer);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_FREE(ret);
    return nullptr;
}

int OpenFileWindow::strnicmp(const char* str1, const char* str2, int n)
{
    int d = 0;
    while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1)
    {
        str1++;
        str2++;
        n--;
    }
    return d;
}


void OpenFileWindow::showOpenFileWindow(bool* opened)
{
    auto visibleSize = _director->getVisibleSize();
    
    Vec2 windowSize = Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.7f);
    ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y));
    ImGui::SetNextWindowPos(ImVec2((visibleSize.width - windowSize.x) / 2, (visibleSize.height - windowSize.y) / 2), ImGuiSetCond_Appearing);
    if (!ImGui::Begin("Open Map", opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
    {
        ImGui::End();
        return;
    }
    
    float height = ImGui::CalcTextSize("").y;
    
    ImGui::InputText("search directory", _filePath, 256);
    
    ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
    ImGui::BeginChild("##Directories", ImVec2(0, visibleSize.height * 0.7 - 25 - height - 50), true);
    
    showDirectoryAndFile(_filePath);
    
    ImGui::EndChild();
    ImGui::PopStyleVar();
    
    if (ImGui::Button("Open", ImVec2(50,25)))
    {
        *opened = false;
        closeWindow();
    }
    ImGui::SameLine();
    if (ImGui::Button("Close", ImVec2(50,25)))
    {
        *opened = false;
        closeWindow();
    }
   
    ImGui::End();
    
    if ( *opened == false)
    {
        closeWindow();
    }
}


void OpenFileWindow::closeWindow()
{
    _imguiLayer->setEnableFileMenu(true);
    _inDirectories.clear();
}


void OpenFileWindow::showDirectoryAndFile(const std::string& path)
{
    auto items = FileSystem::getFilesAndDirectoriesInPath(path);
    for( auto item : items)
    {
        try
        {
            if ( boost::filesystem::is_directory(item) )
            {
                if (ImGui::TreeNode(item.c_str()))
                {
                    showDirectoryAndFile(item);
                    ImGui::TreePop();
                }
            }
            else
            {
                if ( item.size() > 4 && item.substr(item.size() - 4, 4) == ".gmx" )
                {
                    if (ImGui::Selectable(item.c_str()))
                    {
                        strncpy(_filePath, item.c_str(), 256);
                    }
                }
            }
        }
        catch (std::exception& e)
        {
            // ignore
        }
    }
}








