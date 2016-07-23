//
//  NewFileWindow.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 24..
//
//

#include <functional>

#include "NewFileWindow.hpp"
#include "EditScene.hpp"
#include "GMXLayer.hpp"
#include "GMXFile.hpp"
#include "FileSystem.hpp"
using namespace cocos2d;
using namespace realtrick;

NewFileWindow::NewFileWindow(EditScene* layer) :
_imguiLayer(layer)
{
}


NewFileWindow::~NewFileWindow()
{
}


NewFileWindow* NewFileWindow::create(EditScene* layer)
{
    auto ret = new (std::nothrow) NewFileWindow(layer);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_FREE(ret);
    return nullptr;
}


void NewFileWindow::showNewFileWindow(bool* opened)
{
    ImGuiContext& g = *GImGui;
    
    ImGui::SetNextWindowPos(ImVec2((g.IO.DisplaySize.x - 430) / 2, (g.IO.DisplaySize.y - 430) / 2), ImGuiSetCond_Once);
    ImGui::OpenPopup("New");
    if (ImGui::BeginPopupModal("New", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        _imguiLayer->enableModal(true);
        
        ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
        ImGui::BeginChild("##TileSize", ImVec2(0,60), true);
        ImGui::Text("Tile Size");
        ImGui::Columns(2, "##col1", false);
        
        const char* items1[] = {"128"};
        ImGui::Combo("width", &_tileSizeXItem, items1, 1);
        ImGui::NextColumn();
        
        const char* items2[] = {"128"};
        ImGui::Combo("height", &_tileSizeYItem, items2, 1);
        
        ImGui::EndChild();
        ImGui::PopStyleVar();
        
        ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
        ImGui::BeginChild("##Number Of Tiles", ImVec2(0,60), true);
        
        ImGui::Text("Number Of Tiles");
        ImGui::Columns(2, "##col2", false);
        
        const char* items3[] = {"32", "64", "128", "192", "256" };
        ImGui::Combo("x", &_numOfTileX, items3, 5);
        ImGui::NextColumn();
        
        const char* items4[] = {"32", "64", "128", "192", "256" };
        ImGui::Combo("y", &_numOfTileY, items4, 5);
        
        ImGui::EndChild();
        ImGui::PopStyleVar();
        
        ImGui::Text("World size is (%d) x (%d)",
                    atoi(items1[_tileSizeXItem]) * atoi(items3[_numOfTileX]),
                    atoi(items2[_tileSizeYItem]) * atoi(items4[_numOfTileY]));
        
        ImGui::Text("\n");
        
        ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
        ImGui::BeginChild("##Default Tile", ImVec2(0,180), true);
        
        ImGui::Text("Default Tile");
        
        const char* tiles[] = { "Dirt", "Grass", "Water", "Hill" };
        ImGui::ListBox("select\ndefault tile", &_currentTile, tiles, 4, 6);
        
        ImGui::EndChild();
        ImGui::PopStyleVar();
        
        static int nextNumber = 0;
        
        if (ImGui::Button("Create"))
        {
            // create map
            GMXFile* file = new GMXFile();
            
            file->fileName = "untitled_map_" + std::to_string(nextNumber++);
            file->tileWidth = atoi(items1[_tileSizeXItem]);
            file->tileHeight = atoi(items2[_tileSizeYItem]);
            file->numOfTileX = atoi(items3[_numOfTileX]);
            file->numOfTileY = atoi(items4[_numOfTileY]);
            file->worldSize = Size(file->tileWidth * file->numOfTileX, file->tileHeight * file->numOfTileY);
            file->defaultTile = _currentTile;
            
            int x = file->numOfTileX + DUMMY_TILE_SIZE * 2;
            int y = file->numOfTileY * 2 + DUMMY_TILE_SIZE * 4;
            
            file->tileInfos.resize(y);
            for(int i = 0 ; i < y ; ++ i)
            {
                file->tileInfos[i].resize(x);
            }
            
            for(int i = 0 ; i < y; ++ i)
            {
                for(int j = 0 ; j < x ; ++ j)
                {
                    std::string tileName;
                    
                    if ( _currentTile == static_cast<int>(EditorTileType::DIRT)) tileName = "1_" + std::to_string(random(1, 3)) + "_1234";
                    else if ( _currentTile == static_cast<int>(EditorTileType::GRASS)) tileName = "2_" + std::to_string(random(1, 3)) + "_1234";
                    else if ( _currentTile == static_cast<int>(EditorTileType::WATER)) tileName = "3_" + std::to_string(random(1, 3)) + "_1234";
                    else if ( _currentTile == static_cast<int>(EditorTileType::HILL)) tileName = "5_" + std::to_string(random(1, 3)) + "_1234";
                    
                    file->tileInfos[i][j] = tileName;
                }
            }
            
            _imguiLayer->createGMXLayer(file);
            
            *opened = false;
            closeWindow();
        }
        
        ImGui::SameLine();
        if ( ImGui::Button("Cancel") )
        {
            *opened = false;
            closeWindow();
        }

        ImGui::EndPopup();
    }
}


void NewFileWindow::closeWindow()
{
    ImGui::CloseCurrentPopup();
    _imguiLayer->enableModal(false);
    
    _tileSizeXItem = 0;
    _tileSizeYItem = 0;
    _numOfTileX = 0;
    _numOfTileY = 0;
    _currentTile = 0;
}









