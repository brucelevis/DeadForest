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
#include "PaletteLayer.hpp"
#include "SizeProtocol.h"
using namespace realtrick;

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
                if (ImGui::MenuItem("New", "Ctrl+N", &_showNewMap, _enableNewMap))
                {
                    if ( _showNewMap ) doNewButton();
                }
                if (ImGui::MenuItem("Open", "Ctrl+O", false, _enableOpenMap)) {}
                if (ImGui::BeginMenu("Open Recent"))
                {
                    ImGui::MenuItem("fish_hat.c");
                    ImGui::MenuItem("fish_hat.inl");
                    ImGui::MenuItem("fish_hat.h");
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Save", "Ctrl+S", false, _enableSaveMap)) { }
                if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S", false, _enableSaveMap)) { }
                ImGui::Separator();
                if (ImGui::MenuItem("Quit", "Alt+F4")) {}
                
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit", _isEditEnable))
            {
                if ( ImGui::MenuItem("Undo", "CTRL+Z", false, isUndo()) ) { _layer->undo(); }
                if ( ImGui::MenuItem("Redo", "CTRL+Y", false, isRedo()) ) { _layer->redo(); }
                
                ImGui::Separator();
                if ( ImGui::MenuItem("Cut", "CTRL+X", false, false) ) {}
                if ( ImGui::MenuItem("Copy", "CTRL+C", false, false) ) {}
                if ( ImGui::MenuItem("Paste", "CTRL+V", false, false) ) {}
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
                if (ImGui::MenuItem("Player Setting")) {}
                if (ImGui::MenuItem("Force Setting")) {}
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Windows", _isWindowEnable))
            {
                ImGui::MenuItem("Navigator", "SHIFT+N", &_layer->isShowNavigator());
                ImGui::MenuItem("Palette", "SHIFT+P", &_layer->isShowPalette());
                ImGui::MenuItem("History", "SHIFT+H", &_layer->isShowHistory());
                ImGui::MenuItem("Trigger", "SHIFT+T");
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

        ImGuiState& g = *GImGui;
        float height = g.FontBaseSize + g.Style.FramePadding.y * 2.0f;
        
        ImGuiIO& io = ImGui::GetIO();
        
        ImGui::SetNextWindowPos(ImVec2(0.0f, height));
        ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, ICONBAR_HEIGHT));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35, 0.35, 0.35, 0.35));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35, 0.35, 0.35, 0.55));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(219/255.0, 219/255.0, 219/255.0, 1.0));

        ImGui::Begin("##IconMenuBar", NULL,
                     ImGuiWindowFlags_NoTitleBar|
                     ImGuiWindowFlags_NoResize|
                     ImGuiWindowFlags_NoMove|
                     ImGuiWindowFlags_NoScrollbar|
                     ImGuiWindowFlags_NoSavedSettings|
                     ImGuiWindowFlags_NoBringToFrontOnFocus|
                     ImGuiWindowFlags_ShowBorders);
        
        static float newAlpha;
        if ( _enableNewMap )
        {
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35, 0.35, 0.35, 0.35));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35, 0.35, 0.35, 0.55));
            newAlpha = 1.0f;
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0, 0.0, 0.0, 0.0));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0, 0.0, 0.0, 0.0));
            newAlpha = 0.2f;
        }
        if ( ImGuiLayer::imageButton("new.png", 20, 20, ImVec2(0,0), ImVec2(1,1), -1, ImVec4(0,0,0,0), ImVec4(1, 1, 1, newAlpha)) )
        {
            doNewButton();
        }
        ImGui::PopStyleColor(2);
        
        static float openAlpha;
        if ( _enableOpenMap )
        {
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35, 0.35, 0.35, 0.35));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35, 0.35, 0.35, 0.55));
            openAlpha = 1.0f;
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0, 0.0, 0.0, 0.0));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0, 0.0, 0.0, 0.0));
            openAlpha = 0.2f;
        }
        ImGui::SameLine(); ImGuiLayer::imageButton("open.png", 20, 20, ImVec2(0,0), ImVec2(1,1), -1, ImVec4(0,0,0,0), ImVec4(1, 1, 1, openAlpha));
        ImGui::PopStyleColor(2);
        
        static float saveAlpha;
        if ( _enableSaveMap )
        {
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35, 0.35, 0.35, 0.35));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35, 0.35, 0.35, 0.55));
            saveAlpha = 1.0f;
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0, 0.0, 0.0, 0.0));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0, 0.0, 0.0, 0.0));
            saveAlpha = 0.2f;
        }
        ImGui::SameLine(); ImGuiLayer::imageButton("save.png", 20, 20, ImVec2(0,0), ImVec2(1,1), -1, ImVec4(0,0,0,0), ImVec4(1, 1, 1, saveAlpha));
        ImGui::PopStyleColor(2);
        
        ImGui::SameLine();
        static float undoAlpha;
        if ( isUndo() )
        {
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35, 0.35, 0.35, 0.35));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35, 0.35, 0.35, 0.55));
            undoAlpha = 1.0f;
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0, 0.0, 0.0, 0.0));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0, 0.0, 0.0, 0.0));
            undoAlpha = 0.2f;
        }
        if (ImGuiLayer::imageButton("undo.png", 20, 20, ImVec2(0,0), ImVec2(1,1), -1, ImVec4(0,0,0,0), ImVec4(1, 1, 1, undoAlpha)))
        {
            if ( isUndo() ) _layer->undo();
        }
        ImGui::PopStyleColor(2);
        
        ImGui::SameLine();
        static float redoAlpha;
        if ( isRedo() )
        {
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35, 0.35, 0.35, 0.35));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35, 0.35, 0.35, 0.55));
            redoAlpha = 1.0f;
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0, 0.0, 0.0, 0.0));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0, 0.0, 0.0, 0.0));
            redoAlpha = 0.2f;
        }
        if (ImGuiLayer::imageButton("redo.png", 20, 20, ImVec2(0,0), ImVec2(1,1), -1, ImVec4(0,0,0,0), ImVec4(1, 1, 1, redoAlpha)))
        {
            if ( isRedo() ) _layer->redo();
        }
        ImGui::PopStyleColor(2);
        
        ImGui::SameLine();
        ImGui::PushItemWidth(200);
        if (ImGui::Combo("##layer", &_layerType, "Tile Layer\0Entity Layer\0Doodad Layer\0Location Layer\0", 4))
        {
            if ( _layer )
            {
                _layer->getPaletteLayer()->setSelectedItem(-1);
            }
        }
        
        ImGui::SameLine();
        static int playerType = -1;
        if (ImGui::Combo("##player", &playerType, "Player 1\0Player 2\0Player 3\0Player 4\0Player 5\0Player 6\0Player 7\0Player 8\0", 8))
        {
            if ( _layer ) { /* ... */ }
        }
        ImGui::PopItemWidth();
        
        ImGui::End();
        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar(1);

        
        ImGui::SetNextWindowPos(ImVec2(0.0f, io.DisplaySize.y - STATUSBAR_HEIGHT));
        ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, STATUSBAR_HEIGHT));
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
    revertNewButton();
    
    setEnableEditMenu(true);
    setEnablePlayerMenu(true);
    setEnableWindowMenu(true);
}


void EditScene2::doNewButton()
{
    setEnableFileMenu(false);
    setEnableEditMenu(false);
    setEnablePlayerMenu(false);
    setEnableWindowMenu(false);
    
    _showNewMap = true;
    _enableOpenMap = false;
    _enableNewMap = false;
    _enableSaveMap = false;
    _enableUndo = false;
    _enableRedo = false;
}


void EditScene2::revertNewButton()
{
    setEnableFileMenu(true);
    setEnableEditMenu(true);
    setEnablePlayerMenu(true);
    setEnableWindowMenu(true);
    _enableOpenMap = true;
    _enableNewMap = true;
    _enableSaveMap = true;
    _enableRedo = true;
    _enableUndo = true;
}


bool EditScene2::isUndo()
{
    return (_layer &&  _layer->isUndo() && _enableUndo);
}

bool EditScene2::isRedo()
{
    return (_layer &&  _layer->isRedo() && _enableRedo);
}









