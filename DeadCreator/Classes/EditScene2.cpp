//
//  EditScene2.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 26..
//
//

#include "ui/CocosGUI.h"
using namespace cocos2d;

#include "EditScene2.hpp"
#include "GMXLayer2.hpp"
#include "GMXFile.hpp"
#include "NewFileWindow2.hpp"
#include "SizeProtocol.h"

Scene* EditScene2::createScene()
{
    auto scene = Scene::create();
    auto layer = EditScene2::create();
    scene->addChild(layer);
    return scene;
}


bool EditScene2::init()
{
    if ( !ImGuiLayer::init() )
        return false;
    
    GMXFile* file = new GMXFile();
    file->numOfTileY = 256;
    file->numOfTileX = 256;
    file->tileWidth = 128;
    file->tileHeight = 128;
    file->worldSize = Size(file->numOfTileX * file->tileWidth, file->numOfTileY * file->tileHeight);
    
    _newFileWindow = NewFileWindow2::create(this);
    addChild(_newFileWindow);
    
    addImGUI([this] {
        
        if ( _showNewMap ) _newFileWindow->showNewFileWindow(&_showNewMap);
        
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File", _isFileEnable))
            {
                if (ImGui::MenuItem("New", "Ctrl+N", &_showNewMap))
                {
                    if ( _showNewMap ) _isFileEnable = false;
                }
                if (ImGui::MenuItem("Open", "Ctrl+O")) {}
                if (ImGui::BeginMenu("Open Recent"))
                {
                    ImGui::MenuItem("fish_hat.c");
                    ImGui::MenuItem("fish_hat.inl");
                    ImGui::MenuItem("fish_hat.h");
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) { }
                if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S")) { }
                ImGui::Separator();
                if (ImGui::MenuItem("Quit", "Alt+F4")) {}
                
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit", _isEditEnable))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) { }
                if (ImGui::MenuItem("Redo", "CTRL+Y")) { }
                
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X", false, false)) {}
                if (ImGui::MenuItem("Copy", "CTRL+C", false, false)) {}
                if (ImGui::MenuItem("Paste", "CTRL+V", false, false)) {}
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Players", _isPlayerEnable))
            {
                if (ImGui::MenuItem("Player 1")) {}
                if (ImGui::MenuItem("Player 2")) {}
                if (ImGui::MenuItem("Player 3")) {}
                if (ImGui::MenuItem("Player 4")) {}
                if (ImGui::MenuItem("Player 5")) {}
                if (ImGui::MenuItem("Player 6")) {}
                if (ImGui::MenuItem("Player 7")) {}
                if (ImGui::MenuItem("Player 8")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Setting")) {}
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Windows", _isWindowEnable))
            {
                ImGui::MenuItem("Navigator", "SHIFT+N", &_layer->isShowNavigator());
                ImGui::MenuItem("Trigger", "SHIFT+T");
                ImGui::MenuItem("Palette", "SHIFT+P", &_layer->isShowPalette());
                ImGui::MenuItem("Property", "SHIFT+R");
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Help"))
            {
                if (ImGui::MenuItem("Contact")) {}
                if (ImGui::MenuItem("About")) {}
                ImGui::EndMenu();
            }
            
            ImGui::EndMainMenuBar();
        }
        
        ImGuiIO& io = ImGui::GetIO();
        
        ImGui::SetNextWindowPos(ImVec2(0.0f, io.DisplaySize.y - SizeProtocol::STATUSBAR_HEIGHT));
        ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, SizeProtocol::STATUSBAR_HEIGHT));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::Begin("##BottomMenuBar", NULL,
                     ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoSavedSettings);
        
        ImGui::Columns(4, "extra_info", false);
        
        static Vec2 worldPosition = Vec2::ZERO;
        if ( _layer ) worldPosition = _layer->getMousePosInWorld();
        ImGui::Text("World Position (%.0f, %.0f)", worldPosition.x, worldPosition.y);
        
        ImGui::SameLine();
        ImGui::NextColumn();
        ImGui::Text("Player 1");
        
        ImGui::SameLine();
        ImGui::NextColumn();
        ImGui::Text("Object 372");
        
        ImGui::SameLine();
        ImGui::NextColumn();
        ImGui::Text("(%.2f FPS)", ImGui::GetIO().Framerate);
        
        ImGui::End();
        ImGui::PopStyleVar(1);
        
    }, "##main menu");
    
    
    addImGUI([this]{
        
        static bool isShowDemo = true;
        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
        ImGui::ShowTestWindow(&isShowDemo);
        
    }, "test_window");
    
    return true;
}


void EditScene2::createGMXLayer(GMXFile* file)
{
    _layer = GMXLayer2::create(*this, *file);
    addChild(_layer);
    
    _isEditEnable = true;
    _isPlayerEnable = true;
    _isWindowEnable = true;
}









