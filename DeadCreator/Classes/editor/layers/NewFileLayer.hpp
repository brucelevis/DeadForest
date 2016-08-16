//
//  NewFileLayer.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 6. 24..
//
//

#pragma once

#include "cocos2d.h"
#include "EditScene.hpp"
#include "GMXFile.hpp"
#include "StringHelper.hpp"

#include "imgui.h"
#include "imgui_internal.h"

namespace realtrick
{
    namespace editor
    {
        
        class EditScene;
        
        class NewFileLayer : public cocos2d::Node
        {
            
        public:
            
            explicit NewFileLayer(EditScene* layer) : _imguiLayer(layer)
            {
                for(int i = 0 ; i < 8 ; ++ i) _playerOwners[i] = 0;
            }
            virtual ~NewFileLayer() = default;
            static NewFileLayer* create(EditScene* layer)
            {
                auto ret = new (std::nothrow) NewFileLayer(layer);
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
                ImGuiContext& g = *GImGui;
                ImGui::SetNextWindowSize(ImVec2(790, 400));
                ImGui::SetNextWindowPos(ImVec2((g.IO.DisplaySize.x - 790) / 2, (g.IO.DisplaySize.y - 400) / 2), ImGuiSetCond_Once);
                ImGui::OpenPopup("New");
                if (ImGui::BeginPopupModal("New", NULL, ImGuiWindowFlags_NoResize))
                {
                    _imguiLayer->enableModal(true);
                    
                    ImGui::BeginGroup();
                    ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
                    ImGui::BeginChild("##TileSize", ImVec2(400, 60), true);
                    ImGui::Text("Tile Size");
                    ImGui::Columns(2, "##col1", false);
                    
                    const char* items1[] = {"128"};
                    ImGui::PushItemWidth(100);
                    ImGui::Combo("width", &_tileSizeXItem, items1, 1);
                    ImGui::PopItemWidth();
                    ImGui::NextColumn();
                    
                    const char* items2[] = {"128"};
                    ImGui::PushItemWidth(100);
                    ImGui::Combo("height", &_tileSizeYItem, items2, 1);
                    ImGui::PopItemWidth();
                    
                    ImGui::EndChild();
                    ImGui::PopStyleVar();
                    
                    ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
                    ImGui::BeginChild("##Number Of Tiles", ImVec2(400, 60), true);
                    
                    ImGui::Text("Number Of Tiles");
                    ImGui::Columns(2, "##col2", false);
                    
                    const char* items3[] = {"32", "64", "128", "192", "256" };
                    ImGui::PushItemWidth(100);
                    ImGui::Combo("x", &_numOfTileX, items3, 5);
                    ImGui::PopItemWidth();
                    ImGui::NextColumn();
                    
                    const char* items4[] = {"32", "64", "128", "192", "256" };
                    ImGui::PushItemWidth(100);
                    ImGui::Combo("y", &_numOfTileY, items4, 5);
                    ImGui::PopItemWidth();
                    
                    ImGui::EndChild();
                    ImGui::PopStyleVar();
                    
                    ImGui::Text("World size is (%d) x (%d)",
                                atoi(items1[_tileSizeXItem]) * atoi(items3[_numOfTileX]),
                                atoi(items2[_tileSizeYItem]) * atoi(items4[_numOfTileY]));
                    
                    ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
                    ImGui::BeginChild("##Default Tile", ImVec2(400, 180), true);
                    
                    ImGui::Text("Default Tile");
                    
                    const char* tiles[] = { "Dirt", "Grass", "Water", "Hill" };
                    ImGui::ListBox("select\ndefault tile", &_currentTile, tiles, 4, 6);
                    
                    ImGui::EndChild();
                    ImGui::PopStyleVar();
                    ImGui::EndGroup();
                    
                    ImGui::SameLine();
                    ImGui::BeginGroup();
                    ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
                    ImGui::BeginChild("Player Setting", ImVec2(370, 328), true);
                    ImGui::Text("Player Setting");
                    ImGui::Separator();
                    for(int i = 0 ; i < 8 ; ++ i)
                    {
                        ImGui::PushID(i);
                        ImGui::TextUnformatted(std::string("Player " + _to_string(i + 1)).c_str());
                        ImGui::SameLine();
                        ImGui::PushItemWidth(265);
                        ImGui::Combo("", &_playerOwners[i], "Human\0Computer\0Unused\0");
                        ImGui::PopItemWidth();
                        ImGui::PopID();
                    }
                    ImGui::EndChild();
                    ImGui::PopStyleVar();
                    ImGui::EndGroup();
                    
                    if (ImGui::Button("Create"))
                    {
                        // create map
                        GMXFile* file = new GMXFile();
                        
                        static int nextNumber = 0;
                        file->fileName = "untitled_map_" + _to_string(nextNumber++);
                        file->tileWidth = atoi(items1[_tileSizeXItem]);
                        file->tileHeight = atoi(items2[_tileSizeYItem]);
                        file->numOfTileX = atoi(items3[_numOfTileX]);
                        file->numOfTileY = atoi(items4[_numOfTileY]);
                        file->worldSize = cocos2d::Size(file->tileWidth * file->numOfTileX, file->tileHeight * file->numOfTileY);
                        file->defaultTile = _currentTile;
                        
                        int x = file->numOfTileX + DUMMY_TILE_SIZE * 2;
                        int y = file->numOfTileY * 2 + DUMMY_TILE_SIZE * 4;
                        
                        file->tileInfos.resize(y);
                        for(int i = 0 ; i < y ; ++ i)
                            file->tileInfos[i].resize(x);
                        
                        for(int i = 0 ; i < y; ++ i)
                        {
                            for(int j = 0 ; j < x ; ++ j)
                            {
                                std::string tileName;
                                
                                if ( _currentTile == static_cast<int>(TileType::DIRT)) tileName = "1_" + _to_string(cocos2d::random(1, 3)) + "_1234";
                                else if ( _currentTile == static_cast<int>(TileType::GRASS)) tileName = "2_" + _to_string(cocos2d::random(1, 3)) + "_1234";
                                else if ( _currentTile == static_cast<int>(TileType::WATER)) tileName = "3_" + _to_string(cocos2d::random(1, 3)) + "_1234";
                                else if ( _currentTile == static_cast<int>(TileType::HILL)) tileName = "5_" + _to_string(cocos2d::random(1, 3)) + "_1234";
                                
                                file->tileInfos[i][j] = tileName;
                            }
                        }
                        
                        for(int i = 1 ; i <= 8 ; ++ i)
                        {
                            file->playerInfos[i] = PlayerInfo(static_cast<PlayerType>(i),
                                                              Force::FORCE_1,
                                                              static_cast<Owner>(_playerOwners[i]));
                        }
                        
                        std::strncpy(file->force1.name.data(), "Force 1", 20);
                        file->force1.isAlly = true;
                        file->force1.isVision = true;
                        
                        std::strncpy(file->force2.name.data(), "Force 2", 20);
                        file->force2.isAlly = true;
                        file->force2.isVision = true;
                        
                        _imguiLayer->createGMXLayer(file);
                        
                        opened = false;
                        closeWindow();
                    }
                    
                    ImGui::SameLine();
                    if ( ImGui::Button("Cancel") )
                    {
                        opened = false;
                        closeWindow();
                    }
                    
                    ImGui::EndPopup();
                }
            }
            
        private:
            
            void closeWindow()
            {
                ImGui::CloseCurrentPopup();
                _imguiLayer->enableModal(false);
                
                _tileSizeXItem = 0;
                _tileSizeYItem = 0;
                _numOfTileX = 0;
                _numOfTileY = 0;
                _currentTile = 0;
            }
            
        private:
            
            EditScene* _imguiLayer;
            
            int _tileSizeXItem = 0;
            int _tileSizeYItem = 0;
            int _numOfTileX = 0;
            int _numOfTileY = 0;
            int _currentTile = 0;
            
            int _playerOwners[8];
            
        };
        
    }
}









