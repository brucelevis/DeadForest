//
//  EditScene.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 26..
//
//

#include <boost/filesystem.hpp>
using namespace boost::filesystem;

#include "ui/CocosGUI.h"
using namespace cocos2d;

#include "EditScene.hpp"
#include "GMXLayer.hpp"
#include "GMXFile.hpp"
#include "NewFileWindow.hpp"
#include "SaveAsLayer.hpp"
#include "OpenLayer.hpp"
#include "PaletteLayer.hpp"
#include "LocationNode.hpp"
#include "SizeProtocol.h"
using namespace realtrick;

#include "GMXFile_generated.h"
#include "util.h"

Scene* EditScene::createScene()
{
    auto scene = Scene::create();
    auto layer = EditScene::create();
    scene->addChild(layer);
    return scene;
}


bool EditScene::init()
{
    if ( !ImGuiLayer::init() )
        return false;
    
    GMXFile* file = new GMXFile();
    file->numOfTileY = 256;
    file->numOfTileX = 256;
    file->tileWidth = 128;
    file->tileHeight = 128;
    file->worldSize = Size(file->numOfTileX * file->tileWidth, file->numOfTileY * file->tileHeight);
    
    _newFileWindow = NewFileWindow::create(this);
    addChild(_newFileWindow);
    
    _saveAsLayer = SaveAsLayer::create(this);
    addChild(_saveAsLayer);
    
    _openLayer = OpenLayer::create(this);
    addChild(_openLayer);
    
    addImGUI([this] {
        
        if ( _showNewMap ) _newFileWindow->showNewFileWindow(&_showNewMap);
        if ( _showSaveAs ) _saveAsLayer->showLayer(&_showSaveAs);
        if ( _showOpenMap ) _openLayer->showLayer(&_showOpenMap);
        
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File", _isFileEnable))
            {
                if (ImGui::MenuItem("New", "Ctrl+N", &_showNewMap, true)) { doNewButton(); }
                if (ImGui::MenuItem("Open", "Ctrl+O", false, _enableOpenMap)) { doOpenButton(); }
                if (ImGui::MenuItem("Save", "Ctrl+S", false, _enableSaveMap)) { saveFile(_layer->getCurrFilePath()); }
                if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S", &_showSaveAs, _enableSaveMap)) { saveAsFile(); }
                
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
                if (ImGui::MenuItem("Player 1")) { _selectedPlayerType = static_cast<int>(PlayerType::PLAYER1); }
                if (ImGui::MenuItem("Player 2")) { _selectedPlayerType = static_cast<int>(PlayerType::PLAYER2); }
                if (ImGui::MenuItem("Player 3")) { _selectedPlayerType = static_cast<int>(PlayerType::PLAYER3); }
                if (ImGui::MenuItem("Player 4")) { _selectedPlayerType = static_cast<int>(PlayerType::PLAYER4); }
                if (ImGui::MenuItem("Player 5")) { _selectedPlayerType = static_cast<int>(PlayerType::PLAYER5); }
                if (ImGui::MenuItem("Player 6")) { _selectedPlayerType = static_cast<int>(PlayerType::PLAYER6); }
                if (ImGui::MenuItem("Player 7")) { _selectedPlayerType = static_cast<int>(PlayerType::PLAYER7); }
                if (ImGui::MenuItem("Player 8")) { _selectedPlayerType = static_cast<int>(PlayerType::PLAYER8); }
                
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
                ImGui::MenuItem("Trigger", "SHIFT+T", &_layer->isShowTriggerEdit());
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
        
        ImGuiContext& g = *GImGui;
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
        
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35, 0.35, 0.35, 0.35));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35, 0.35, 0.35, 0.55));
        if ( ImGuiLayer::imageButton("new.png", 20, 20,
                                     ImVec2(0,0),
                                     ImVec2(1,1), -1,
                                     ImVec4(0,0,0,0),
                                     ImVec4(1, 1, 1, 1.0)) )
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
        ImGui::SameLine();
        if (ImGuiLayer::imageButton("open.png", 20, 20,
                                    ImVec2(0,0),
                                    ImVec2(1,1), -1, ImVec4(0,0,0,0), ImVec4(1, 1, 1, openAlpha)))
        {
            doOpenButton();
        }
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
        ImGui::SameLine();
        if (ImGuiLayer::imageButton("save.png", 20, 20,
                                    ImVec2(0,0),
                                    ImVec2(1,1), -1, ImVec4(0,0,0,0), ImVec4(1, 1, 1, saveAlpha)) )
        {
            doSaveButton();
        }
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
            if ( _layer ) _layer->getPaletteLayer()->setSelectedItem(-1);
            changeLayerType( static_cast<LayerType>(_layerType) );
        }
        
        ImGui::SameLine();
        if (ImGui::Combo("##player", &_selectedPlayerType,
                         "Player 1\0Player 2\0Player 3\0Player 4\0Player 5\0Player 6\0Player 7\0Player 8\0", 8))
        {}
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


void EditScene::createGMXLayer(GMXFile* file)
{
    if ( _layer )
    {
        _layer->removeFromParent();
        log("remove from parent");
    }
    
    _layer = GMXLayer::create(*this, *file);
    addChild(_layer);
    
    // menu
    setEnableEditMenu(true);
    setEnablePlayerMenu(true);
    setEnableWindowMenu(true);
    
    // button
    setEnableSaveButton(true);
    
    _selectedPlayerType = 0;
}


void EditScene::createGMXLayer(const std::string& filePath)
{
    if ( _layer ) _layer->removeFromParent();
    
    std::string loadData;
    auto ret =  flatbuffers::LoadFile(filePath.c_str(), true, &loadData);
    if ( ret )
    {
        auto gmxFile = DeadCreator::GetGMXFile(loadData.c_str());
        
        auto file = new GMXFile();
        file->fileName = path(filePath).leaf().native();
        file->defaultTile = gmxFile->default_type();
        file->numOfTileX = gmxFile->number_of_tiles()->x();
        file->numOfTileY = gmxFile->number_of_tiles()->y();
        file->tileWidth = gmxFile->tile_size()->width();
        file->tileHeight = gmxFile->tile_size()->height();
        file->worldSize = Size(file->numOfTileX * file->tileWidth, file->numOfTileY * file->tileHeight);
        
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
                
                if ( file->defaultTile == static_cast<int>(EditorTileType::DIRT))
                    tileName = "1_" + std::to_string(random(1, 3)) + "_1234";
                else if ( file->defaultTile == static_cast<int>(EditorTileType::GRASS))
                    tileName = "2_" + std::to_string(random(1, 3)) + "_1234";
                else if ( file->defaultTile == static_cast<int>(EditorTileType::WATER))
                    tileName = "3_" + std::to_string(random(1, 3)) + "_1234";
                else if ( file->defaultTile == static_cast<int>(EditorTileType::HILL))
                    tileName = "5_" + std::to_string(random(1, 3)) + "_1234";
                
                file->tileInfos[i][j] = tileName;
            }
        }
        
        for ( auto iter = gmxFile->tiles()->begin(); iter != gmxFile->tiles()->end() ; ++ iter)
        {
            int xx = iter->indices()->x();
            int yy = iter->indices()->y();
            file->tileInfos[yy][xx] = iter->number()->str();
        }
        
        _layer = GMXLayer::create(*this, *file);
        _layer->setCurrFilePath(filePath);
        _layer->enableFirstFile(false);
        addChild(_layer);
        
        // tile infos
        for ( auto iter = gmxFile->tiles()->begin(); iter != gmxFile->tiles()->end() ; ++ iter)
        {
            int xx = iter->indices()->x();
            int yy = iter->indices()->y();
            TileBase tile = TileBase(xx, yy, iter->number()->str(),
                                     indexToPosition(xx, yy, file->tileWidth, file->tileHeight, DUMMY_TILE_SIZE));
            _layer->setTile(xx, yy, tile, true);
        }
        
        
        // collision regions
        for ( auto iter = gmxFile->collision_regions()->begin(); iter != gmxFile->collision_regions()->end(); ++ iter )
        {
            auto polygon = iter;
            for( auto vert = polygon->vertices()->begin() ; vert != polygon->vertices()->end() ; ++ vert )
            {
                log("[%f, %f]", vert->x(), vert->y());
            }
        }
        
        // entities
        for ( auto iter = gmxFile->entities()->begin(); iter != gmxFile->entities()->end() ; ++ iter )
        {
            Vec2 pos(iter->pos()->x(), iter->pos()->y());
            PlayerType playerType = static_cast<PlayerType>(iter->player_type());
            auto entType = static_cast<EntityType>(iter->entity_type());
            
            EditorEntity* ent = EditorEntity::create(_layer->getNextValidID(), entType);
            ent->setPosition(pos);
            ent->setPlayerType(playerType);
            _layer->addEntityForce(ent, 5);
        }
        
        // locations
        for ( auto iter = gmxFile->locations()->begin() ; iter != gmxFile->locations()->end() ; ++ iter)
        {
            std::string name = iter->name()->str();
            Vec2 pos(iter->pos()->x(), iter->pos()->y());
            std::pair<int, int> size = std::make_pair(iter->size()->width(), iter->size()->height());
            
            LocationNode* loc = LocationNode::create(*_layer);
            loc->setLocationName(name);
            loc->setPositionFromWorldPosition(pos);
            loc->setLocationSize(size.first, size.second);
            loc->setSelected(false);
            _layer->addLocation(loc);
        }
        
        _layer->updateCollisionRegion();
        
        // menu
        setEnableEditMenu(true);
        setEnablePlayerMenu(true);
        setEnableWindowMenu(true);
        
        // button
        setEnableSaveButton(true);
        _selectedPlayerType = 0;
        
        changeLayerType(LayerType::TILE);
    }
}


void EditScene::doNewButton()
{
    if ( _layer ) _layer->updateChunk(_layer->getCameraPosition());
    _showNewMap = true;
}


void EditScene::doOpenButton()
{
    if ( _layer ) _layer->updateChunk(_layer->getCameraPosition());
    _showOpenMap = true;
}


void EditScene::changeLayerType(LayerType type)
{
    _layerType = type;
    if ( _layer )
    {
        if ( type == LayerType::LOCATION ) _layer->setVisibleLocations(true);
        else _layer->setVisibleLocations(false);
        
        if ( type == LayerType::TILE ) _layer->setVisibleCollisionRegions(true);
        else _layer->setVisibleCollisionRegions(false);
        
        if ( type != LayerType::ENTITY ) _layer->clearSelectedEntites();
    }
}


bool EditScene::isUndo()
{
    return (_layer &&  _layer->isUndo());
}


bool EditScene::isRedo()
{
    return (_layer &&  _layer->isRedo());
}


void EditScene::doSaveButton()
{
    if ( _layer )
    {
        _layer->updateChunk(_layer->getCameraPosition());
        if ( _layer->isFirstFile() ) saveAsFile();
        else saveFile(_layer->getCurrFilePath());
    }
}


void EditScene::saveFile(const std::string& filePath)
{
    if ( _layer ) _layer->save(filePath);
}


void EditScene::saveAsFile()
{
    _showSaveAs = true;
}






