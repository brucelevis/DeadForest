////
////  OpenFileWindow.cpp
////  DeadCreator
////
////  Created by mac on 2016. 6. 24..
////
////
//
//#include <functional>
//
//#include "OpenFileWindow.hpp"
//#include "FileSystem.hpp"
//#include "EditScene.h"
//#include "SizeProtocol.h"
//using namespace cocos2d;
//
//#include "imgui_internal.h"
//
//OpenFileWindow::OpenFileWindow(EditScene* layer) :
//_imguiLayer(layer)
//{
//    strncpy(_filePath, "/\0", 2);
//}
//
//
//OpenFileWindow::~OpenFileWindow()
//{
//}
//
//
//OpenFileWindow* OpenFileWindow::create(EditScene* layer)
//{
//    auto ret = new (std::nothrow) OpenFileWindow(layer);
//    if ( ret && ret->init() )
//    {
//        ret->autorelease();
//        return ret;
//    }
//    CC_SAFE_FREE(ret);
//    return nullptr;
//}
//
//
//void OpenFileWindow::showOpenFileWindow(bool* opened)
//{
//    auto visibleSize = _director->getVisibleSize();
//    
//    Vec2 windowSize = Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.7f);
//    ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y));
//    ImGui::SetNextWindowPos(ImVec2((visibleSize.width - windowSize.x) / 2, (visibleSize.height - windowSize.y) / 2), ImGuiSetCond_Appearing);
//    if (!ImGui::Begin("Open", opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
//    {
//        ImGui::End();
//        return;
//    }
//    
//    float height = ImGui::CalcTextSize("").y;
//    
//    ImGui::InputText("search directory", _filePath, 256);
//    
//    ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
//    ImGui::BeginChild("##Directories", ImVec2(0, visibleSize.height * 0.7 - 25 - height - 50), true);
//    
//    showDirectoryAndFile(_filePath);
//    
//    ImGui::EndChild();
//    ImGui::PopStyleVar();
//
//    auto style = ImGui::GetStyle();
//    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(style.Colors[ImGuiCol_Text].x,
//                                                style.Colors[ImGuiCol_Text].y,
//                                                style.Colors[ImGuiCol_Text].z,
//                                                _openButtonTextAlpha));
//    
//    if (ImGui::ButtonEx("Open", ImVec2(50,25), _openButtonFlags))
//    {
//        _imguiLayer->open(_filePath);
//        _lastFileRoot = FileSystem::getParentPath(_filePath);
//        *opened = false;
//        closeWindow();
//    }
//    ImGui::PopStyleColor();
//    
//    ImGui::SameLine();
//    if (ImGui::Button("Close", ImVec2(50,25)))
//    {
//        *opened = false;
//        closeWindow();
//    }
//   
//    ImGui::End();
//    
//    if ( *opened == false)
//    {
//        closeWindow();
//    }
//}
//
//
//void OpenFileWindow::closeWindow()
//{
//    strncpy(_filePath, _lastFileRoot.c_str(), 256);
//    _openButtonTextAlpha = 0.5f;
//    _openButtonFlags |= ImGuiButtonFlags_Disabled;
//    _imguiLayer->setEnableFileMenu(true);
//    _inDirectories.clear();
//}
//
//
//void OpenFileWindow::showDirectoryAndFile(const std::string& path)
//{
//    auto items = FileSystem::getFilesAndDirectoriesInPath(path);
//    for( auto item : items)
//    {
//        try
//        {
//            if ( boost::filesystem::is_directory(item) )
//            {
//                auto idx = item.find_first_of('.');
//                if ( idx != std::string::npos && item[idx-1] == '/' )
//                    continue;
//                
//                if (ImGui::TreeNode(item.c_str()))
//                {
//                    _openButtonTextAlpha = 0.5f;
//                    _openButtonFlags |= ImGuiButtonFlags_Disabled;
//                    showDirectoryAndFile(item);
//                    ImGui::TreePop();
//                }
//            }
//            else
//            {
//                if ( item.size() > 4 && item.substr(item.size() - 4, 4) == ".gmx" )
//                {
//                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
//                    ImGui::Bullet();
//                    if (ImGui::Selectable(item.c_str()))
//                    {
//                        _openButtonTextAlpha = 1.0f;
//                        _openButtonFlags = 0;
//                        strncpy(_filePath, item.c_str(), 256);
//                    }
//                    ImGui::PopStyleColor();
//                }
//            }
//        }
//        catch (std::exception& e)
//        {
//            // ignore
//        }
//    }
//}
//
//
//
//
//
//
//
//
