////
////  SaveAsFileWindow.cpp
////  DeadCreator
////
////  Created by mac on 2016. 6. 26..
////
////
//
//#include "SaveAsFileWindow.hpp"
//#include "EditScene.h"
//#include "FileSystem.hpp"
//using namespace cocos2d;
//
//SaveAsFileWindow::SaveAsFileWindow(EditScene* layer) :
//_imguiLayer(layer)
//{
//    strncpy(_filePath, "/\0", 2);
//}
//
//
//SaveAsFileWindow::~SaveAsFileWindow()
//{
//}
//
//
//SaveAsFileWindow* SaveAsFileWindow::create(EditScene* layer)
//{
//    auto ret = new (std::nothrow) SaveAsFileWindow(layer);
//    if ( ret && ret->init() )
//    {
//        ret->autorelease();
//        return ret;
//    }
//    CC_SAFE_DELETE(ret);
//    return nullptr;
//}
//
//
//bool SaveAsFileWindow::init()
//{
//    if ( !Node::init() )
//        return false;
//    
//    return true;
//}
//
//
//void SaveAsFileWindow::showSaveAsFileWindow(bool* opened)
//{
//    auto visibleSize = _director->getVisibleSize();
//    
//    Vec2 windowSize = Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.7f);
//    ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y));
//    ImGui::SetNextWindowPos(ImVec2((visibleSize.width - windowSize.x) / 2, (visibleSize.height - windowSize.y) / 2), ImGuiSetCond_Appearing);
//    if (!ImGui::Begin("Save As", opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
//    {
//        ImGui::End();
//        return;
//    }
//    
//    float height = ImGui::CalcTextSize("").y;
//    
//    if (ImGui::InputText("search directory", _filePath, 256))
//    {
//        std::string temp = _filePath;
//        auto idx = temp.find_last_of('/');
//        temp = temp.substr(0, idx);
//        temp += '/';
//        
//        if ( FileSystem::isExist(temp) && !FileSystem::isExist(_filePath) )
//        {
//            _saveButtonTextAlpha = 1.0f;
//            _saveButtonFlags = 0;
//        }
//        else
//        {
//            _saveButtonTextAlpha = 0.5f;
//            _saveButtonFlags = ImGuiButtonFlags_Disabled;
//        }
//    }
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
//                                                _saveButtonTextAlpha));
//    
//    if (ImGui::ButtonEx("Save", ImVec2(60,25), _saveButtonFlags))
//    {
//        _imguiLayer->save(std::string(_filePath) + ".gmx");
//        _lastFileRoot = FileSystem::getParentPath(_filePath);
//        *opened = false;
//        closeWindow();
//    }
//    ImGui::PopStyleColor();
//    
//    ImGui::SameLine();
//    if (ImGui::Button("Close", ImVec2(60,25)))
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
//void SaveAsFileWindow::closeWindow()
//{
//    strncpy(_filePath, _lastFileRoot.c_str(), 256);
//    _saveButtonTextAlpha = 0.5f;
//    _saveButtonFlags = ImGuiButtonFlags_Disabled;
//    _imguiLayer->setEnableFileMenu(true);
//    _imguiLayer->setEnableEditMenu(true);
//    _imguiLayer->setEnablePlayerMenu(true);
//    _imguiLayer->setEnableWindowMenu(true);
//    _inDirectories.clear();
//}
//
//void SaveAsFileWindow::showDirectoryAndFile(const std::string& path)
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
//                if ( ImGui::TreeNode(item.c_str()) )
//                {
//                    _saveButtonTextAlpha = 0.5f;
//                    _saveButtonFlags = ImGuiButtonFlags_Disabled;
//                    strncpy(_filePath, item.c_str(), 256);
//                    showDirectoryAndFile(item);
//                    ImGui::TreePop();
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
