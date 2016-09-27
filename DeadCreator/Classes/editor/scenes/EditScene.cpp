//
//  EditScene.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 6. 26..
//
//

#include <boost/filesystem.hpp>
using namespace boost::filesystem;

#include "ui/CocosGUI.h"
using namespace cocos2d;

#include "SizeProtocol.h"
#include "EditScene.hpp"
#include "GMXLayer.hpp"
#include "GMXFile.hpp"
#include "NewFileLayer.hpp"
#include "SaveAsLayer.hpp"
#include "OpenLayer.hpp"
#include "SaveQueryLayer.hpp"
#include "PaletteLayer.hpp"
#include "PlayGameLayer.hpp"
#include "LocationNode.hpp"
#include "GameTrigger.hpp"
#include "Conditions.hpp"
#include "Actions.hpp"
#include "StringHelper.hpp"
#include "DummyScene.hpp"
#include "ForceSettingLayer.hpp"
using namespace realtrick;
using namespace realtrick::editor;

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
    
    _newFileLayer = NewFileLayer::create(this);
    addChild(_newFileLayer);
    
    _saveAsLayer = SaveAsLayer::create(this);
    addChild(_saveAsLayer);
    
    _openLayer = OpenLayer::create(this);
    addChild(_openLayer);
    
    _saveQueryLayer = SaveQueryLayer::create(this);
    addChild(_saveQueryLayer);
    
    _playGameLayer = PlayGameLayer::create(this);
    addChild(_playGameLayer);
    
    addImGUI([this] {
        
        _isEditMode = (_layer && !_showPlayGameLayer);
        
        if ( _showNewMap ) _newFileLayer->showLayer(_showNewMap);
        if ( _showSaveAs ) _saveAsLayer->showLayer(_showSaveAs);
        if ( _showOpenMap ) _openLayer->showLayer(_showOpenMap);
        if ( _showSaveQueryLayer ) _saveQueryLayer->showLayer(_showSaveQueryLayer);
        if ( _showPlayGameLayer ) _playGameLayer->showLayer(_showPlayGameLayer);
        if ( _layer && _showGMXLayer && !_showPlayGameLayer ) _layer->showLayer(_showGMXLayer);
        
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File", _isFileEnable && !_showPlayGameLayer))
            {
                if (ImGui::MenuItem("New", "Ctrl+N", &_showNewMap, isNew() )) { doNewButton(); }
                if (ImGui::MenuItem("Open", "Ctrl+O", false, isOpen() )) { doOpenButton(); }
                if (ImGui::MenuItem("Save", "Ctrl+S", false, isSave() )) { saveFile(_layer->getCurrFilePath()); }
                if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S", &_showSaveAs, isSave() )) { saveAsFile(); }
                
                ImGui::Separator();
                if (ImGui::MenuItem("Quit", "Alt+F4")) {}
                
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Edit", _isEditEnable && !_showPlayGameLayer))
            {
                if ( ImGui::MenuItem("Undo", "CTRL+Z", false, isUndo()) ) { _layer->undo(); }
                if ( ImGui::MenuItem("Redo", "CTRL+Y", false, isRedo()) ) { _layer->redo(); }
                
                ImGui::Separator();
                if ( ImGui::MenuItem("Cut", "CTRL+X", false, false) ) {}
                if ( ImGui::MenuItem("Copy", "CTRL+C", false, false) ) {}
                if ( ImGui::MenuItem("Paste", "CTRL+V", false, false) ) {}
                
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Windows", _isWindowEnable && !_showPlayGameLayer))
            {
                ImGui::MenuItem("Navigator", "SHIFT+N", &_layer->isShowNavigator());
                ImGui::MenuItem("Palette", "SHIFT+P", &_layer->isShowPalette());
                ImGui::MenuItem("History", "SHIFT+H", &_layer->isShowHistory());
                ImGui::MenuItem("Trigger", "SHIFT+T", &_layer->isShowTriggerEdit());
                ImGui::MenuItem("Force", "SHIFT+F", &_layer->isShowForceSetting());
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
        
        static float newAlpha;
        if ( isNew() )
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
        if ( ImGuiLayer::imageButton("editor/new.png", 20, 20,
                                     ImVec2(0,0), ImVec2(1,1), -1, ImVec4(0,0,0,0), ImVec4(1, 1, 1, newAlpha)) )
        {
            if ( isNew() ) doNewButton();
        }
        if ( isNew() && ImGui::IsItemHovered()) ImGui::SetTooltip("new");
        ImGui::PopStyleColor(2);
        
        static float openAlpha;
        if ( isOpen() )
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
        if (ImGuiLayer::imageButton("editor/open.png", 20, 20,
                                    ImVec2(0,0),
                                    ImVec2(1,1), -1, ImVec4(0,0,0,0), ImVec4(1, 1, 1, openAlpha)))
        {
            if ( isOpen() ) doOpenButton();
        }
        if ( _enableOpenMap && !_showPlayGameLayer && ImGui::IsItemHovered()) ImGui::SetTooltip("open");
        ImGui::PopStyleColor(2);
        
        static float saveAlpha;
        if ( isSave() )
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
        if (ImGuiLayer::imageButton("editor/save.png", 20, 20,
                                    ImVec2(0,0),
                                    ImVec2(1,1), -1, ImVec4(0,0,0,0), ImVec4(1, 1, 1, saveAlpha)) )
        {
            if ( isSave() ) doSaveButton();
        }
        if ( _enableSaveMap && _isEditMode && ImGui::IsItemHovered()) ImGui::SetTooltip("save");
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
        if (ImGuiLayer::imageButton("editor/undo.png", 20, 20, ImVec2(0,0), ImVec2(1,1), -1, ImVec4(0,0,0,0), ImVec4(1, 1, 1, undoAlpha)))
        {
            if ( isUndo() ) _layer->undo();
        }
        if ( isUndo() && ImGui::IsItemHovered()) ImGui::SetTooltip("undo");
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
        if (ImGuiLayer::imageButton("editor/redo.png", 20, 20, ImVec2(0,0), ImVec2(1,1), -1, ImVec4(0,0,0,0), ImVec4(1, 1, 1, redoAlpha)))
        {
            if ( isRedo() ) _layer->redo();
        }
        if ( isRedo() && ImGui::IsItemHovered()) ImGui::SetTooltip("redo");
        ImGui::PopStyleColor(2);
        
        static float windowAlpha;
        if ( _isEditMode )
        {
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35, 0.35, 0.35, 0.35));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35, 0.35, 0.35, 0.55));
            windowAlpha = 1.0f;
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0, 0.0, 0.0, 0.0));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0, 0.0, 0.0, 0.0));
            windowAlpha = 0.2f;
        }
        
        ImGui::SameLine();
        if (ImGuiLayer::imageButton("editor/navi.png", 20, 20, ImVec2(0,0), ImVec2(1,1),
                                    -1, ImVec4(0,0,0,0), ImVec4(1, 1, 1, windowAlpha)))
        {
            if ( _isEditMode )
            {
                _layer->isShowNavigator() = !_layer->isShowNavigator();
            }
        }
        if ( _isEditMode && ImGui::IsItemHovered()) ImGui::SetTooltip("navigator");
        
        
        ImGui::SameLine();
        if ( ImGuiLayer::imageButton("editor/palette.png", 20, 20, ImVec2(0,0), ImVec2(1,1),
                                     -1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, windowAlpha)))
        {
            if ( _isEditMode )
            {
                _layer->isShowPalette() = !_layer->isShowPalette();
            }
        }
        if ( _isEditMode && ImGui::IsItemHovered()) ImGui::SetTooltip("palette");
        
        ImGui::SameLine();
        if (ImGuiLayer::imageButton("editor/history.png", 20, 20, ImVec2(0,0), ImVec2(1,1),
                                    -1, ImVec4(0,0,0,0), ImVec4(1, 1, 1, windowAlpha)))
        {
            if ( _isEditMode )
            {
                _layer->isShowHistory() = !_layer->isShowHistory();
            }
        }
        if ( _isEditMode && ImGui::IsItemHovered()) ImGui::SetTooltip("history");
        
        ImGui::SameLine();
        if ( ImGuiLayer::imageButton("editor/trigger.png", 20, 20, ImVec2(0,0), ImVec2(1,1),
                                     -1, ImVec4(0,0,0,0), ImVec4(1, 1, 1, windowAlpha)))
        {
            if ( _isEditMode )
            {
                _layer->isShowTriggerEdit() = !_layer->isShowTriggerEdit();
            }
        }
        if ( _isEditMode && ImGui::IsItemHovered()) ImGui::SetTooltip("trigger");
        
        ImGui::SameLine();
        if ( ImGuiLayer::imageButton("editor/play_btn.png", 20, 20, ImVec2(0,0), ImVec2(1,1),
                                     -1, ImVec4(0,0,0,0), ImVec4(1, 1, 1, windowAlpha)))
        {
            if ( _isEditMode )
            {
                _showPlayGameLayer = !_showPlayGameLayer;
                
                if ( _showPlayGameLayer ) playGame();
                else stopGame();
            }
        }
        if ( _isEditMode && ImGui::IsItemHovered()) ImGui::SetTooltip("simulation");
        
        ImGui::PopStyleColor(2);
        
        if ( _layer && !_showPlayGameLayer )
        {
            ImGui::SameLine();
            ImGui::PushItemWidth(250);
            if (ImGui::Combo("##layer", &_layerType, "Tile Layer\0Entity Layer\0Location Layer\0", 4))
            {
                if ( _layer ) _layer->getPaletteLayer()->setSelectedItem(-1);
                changeLayerType( static_cast<LayerType>(_layerType) );
            }
            
            ImGui::SameLine();
            std::string players;
            for ( int i = 1 ; i <= 8 ; ++ i )
            {
                const auto& info = _layer->getFile().playerInfos[i];
                std::string player = "Player " + _to_string(static_cast<int>(info.player));
                std::string owner;
                if ( info.owner == Owner::HUMAN ) owner = "(Human)";
                else if ( info.owner == Owner::COMPUTER ) owner = "(Computer)";
                else if ( info.owner == Owner::UNUSED ) owner = "(Unused)";
                players += (player + " " + owner + '\0');
            }
            static int oldSelectedPlayer = _selectedPlayerType;
            if ( ImGui::Combo("##player", &_selectedPlayerType, players.c_str(), 8) )
            {
                if ( _layer->getFile().playerInfos[_selectedPlayerType + 1].owner == Owner::UNUSED )
                {
                    _selectedPlayerType = oldSelectedPlayer;
                }
                else
                {
                    oldSelectedPlayer = _selectedPlayerType;
                }
            }
            ImGui::PopItemWidth();
        }
        
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
        
        static Vec2 worldPosition = Vec2::ZERO;
        if ( _layer ) worldPosition = _layer->getMousePosInWorld();
        ImGui::Text("Mouse Position (World) (%.0f, %.0f)", worldPosition.x, worldPosition.y);
        
        ImGui::End();
        ImGui::PopStyleVar(1);
        
        //        static bool isShowDemo = true;
        //        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
        //        ImGui::ShowTestWindow(&isShowDemo);
        
    }, "##main menu");
    
    return true;
}


void EditScene::createGMXLayer(GMXFile* file)
{
    if ( _layer ) closeGMXLayer();
    
    _layer = GMXLayer::create(*this, *file);
    addChild(_layer);
    
    _showGMXLayer = true;
    
    // menu
    setEnableEditMenu(true);
    setEnableWindowMenu(true);
    
    // button
    setEnableSaveButton(true);
    
    _selectedPlayerType = 0;
}


void EditScene::createGMXLayer(const std::string& filePath)
{
    if ( _layer ) closeGMXLayer();
    
    std::string loadData;
    auto ret =  flatbuffers::LoadFile(filePath.c_str(), true, &loadData);
    if ( ret )
    {
        auto gmxFile = DeadCreator::GetGMXFile(loadData.c_str());
        
        auto file = new GMXFile();
        file->fileName = path(filePath).leaf().string();
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
                
                if ( file->defaultTile == static_cast<int>(TileType::DIRT))
                    tileName = "1_" + _to_string(random(1, 3)) + "_1234";
                else if ( file->defaultTile == static_cast<int>(TileType::GRASS))
                    tileName = "2_" + _to_string(random(1, 3)) + "_1234";
                else if ( file->defaultTile == static_cast<int>(TileType::WATER))
                    tileName = "3_" + _to_string(random(1, 3)) + "_1234";
                else if ( file->defaultTile == static_cast<int>(TileType::HILL))
                    tileName = "5_" + _to_string(random(1, 3)) + "_1234";
                
                file->tileInfos[i][j] = tileName;
            }
        }

        
        // player infos
        int i = 1;
        for ( auto info = gmxFile->playerInfos()->begin(); info != gmxFile->playerInfos()->end() ; ++ info )
        {
            file->playerInfos[i] = PlayerInfo(static_cast<PlayerType>(info->player()),
                                              static_cast<Force>(info->force()),
                                              static_cast<Owner>(info->owner()));
            ++i;
        }
        
        // force infos
        std::strncpy(file->force1.name.data(), gmxFile->force1_info()->name()->c_str(), 20);
        file->force1.isAlly = gmxFile->force1_info()->is_ally();
        file->force1.isVision = gmxFile->force1_info()->is_vision();
        
        std::strncpy(file->force2.name.data(), gmxFile->force2_info()->name()->c_str(), 20);
        file->force2.isAlly = gmxFile->force2_info()->is_ally();
        file->force2.isVision = gmxFile->force2_info()->is_vision();
        
        std::strncpy(file->force3.name.data(), gmxFile->force3_info()->name()->c_str(), 20);
        file->force3.isAlly = gmxFile->force3_info()->is_ally();
        file->force3.isVision = gmxFile->force3_info()->is_vision();
        
        std::strncpy(file->force4.name.data(), gmxFile->force4_info()->name()->c_str(), 20);
        file->force4.isAlly = gmxFile->force4_info()->is_ally();
        file->force4.isVision = gmxFile->force4_info()->is_vision();
        
        _layer = GMXLayer::create(*this, *file);
        _layer->setCurrFilePath(filePath);
        _layer->enableFirstFile(false);
        addChild(_layer);
        
        _showGMXLayer = true;
        
        
        // tile infos
        for ( auto iter = gmxFile->tiles()->begin(); iter != gmxFile->tiles()->end() ; ++ iter)
        {
            int xx = iter->indices()->x();
            int yy = iter->indices()->y();
            Tileset tile = Tileset(xx, yy, iter->number()->str(),
                                   indexToPosition(xx, yy, file->tileWidth, file->tileHeight, DUMMY_TILE_SIZE));
            _layer->setTile(xx, yy, tile, true);
        }
        
        
        // entities
        for ( auto iter = gmxFile->entities()->begin(); iter != gmxFile->entities()->end() ; ++ iter )
        {
            Vec2 pos(iter->pos()->x(), iter->pos()->y());
            PlayerType playerType = static_cast<PlayerType>(iter->player_type());
            auto entType = static_cast<EntityType>(iter->entity_type());
            
            EditorEntity* ent = EditorEntity::create(iter->id(), entType);
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
        
        
        // triggers
        for ( auto trigger = gmxFile->triggers()->begin(); trigger != gmxFile->triggers()->end() ; ++ trigger )
        {
            GameTrigger* newTrigger = new GameTrigger();
            
            // set players
            for(auto index = trigger->players()->begin(); index != trigger->players()->end(); ++ index)
            {
                newTrigger->isPlayerSelected[*index - 1] = true;
            }
            
            // set conditions
            for(auto cond = trigger->conditions()->begin() ; cond != trigger->conditions()->end(); ++ cond)
            {
                auto condType = cond->condition_type();
                switch (condType)
                {
                    case DeadCreator::ConditionBase_Bring:
                    {
                        auto conditionObject = static_cast<const DeadCreator::Bring*>(cond->condition());
                        auto condition = new ConditionBring();
                        condition->setPlayerType(static_cast<PlayerType>(conditionObject->player()));
                        condition->setApproximation(static_cast<ApproximationType>(conditionObject->approximation()));
                        condition->setEntity(static_cast<EntityType>(conditionObject->entity_type()));
                        condition->setNumber(conditionObject->number());
                        
                        LocationNode* locationPtr = _layer->findLocation(conditionObject->location_name()->str());
                        condition->setLocation(locationPtr);
                        newTrigger->addCondition(condition);
                        
                        break;
                    }
                    case DeadCreator::ConditionBase_Always:
                    {
                        auto condition = new ConditionAlways();
                        newTrigger->addCondition(condition);
                        break;
                    }
                    case DeadCreator::ConditionBase_Never:
                    {
                        auto condition = new ConditionNever();
                        newTrigger->addCondition(condition);
                        break;
                    }
                    default: { cocos2d::log("invalid condition type"); break;}
                }
            }
            
            
            // set actions
            for(auto act = trigger->actions()->begin() ; act != trigger->actions()->end(); ++ act)
            {
                auto actType = act->action_type();
                switch (actType)
                {
                    case DeadCreator::ActionBase_DisplayText:
                    {
                        auto actionObject = static_cast<const DeadCreator::DisplayText*>(act->action());
                        auto action = new ActionDisplayText();
                        action->setText(actionObject->text()->str());
                        newTrigger->addAction(action);
                        
                        break;
                    }
                    case DeadCreator::ActionBase_PreserveTrigger:
                    {
                        auto action = new ActionPreserveTrigger();
                        newTrigger->addAction(action);
                        
                        break;
                    }
                    case DeadCreator::ActionBase_Victory:
                    {
                        auto action = new ActionVictory();
                        newTrigger->addAction(action);
                        
                        break;
                    }
                    case DeadCreator::ActionBase_Defeat:
                    {
                        auto action = new ActionDefeat();
                        newTrigger->addAction(action);
                        
                        break;
                    }
                    case DeadCreator::ActionBase_KillEntityAtLocation:
                    {
                        auto actionObject = static_cast<const DeadCreator::KillEntityAtLocation*>(act->action());
                        auto action = new ActionKillEntityAtLocation();
                        
                        auto numberAll = actionObject->numberAll();
                        if ( numberAll  == -1 /* all */) action->setNumberAll( { true, 0 } );
                        else action->setNumberAll( { false, numberAll } );
                        
                        action->setEntity(static_cast<EntityType>(actionObject->entity_type()));
                        action->setPlayerType(static_cast<PlayerType>(actionObject->player()));
                        
                        LocationNode* locationPtr = _layer->findLocation(actionObject->location_name()->str());
                        action->setLocation(locationPtr);
                        
                        newTrigger->addAction(action);
                        
                        break;
                    }
                    case DeadCreator::ActionBase_MoveLocation:
                    {
                        auto actionObject = static_cast<const DeadCreator::MoveLocation*>(act->action());
                        auto action = new ActionMoveLocation();
                        
                        LocationNode* destLocationPtr = _layer->findLocation(actionObject->dest_location_name()->str());
                        action->setDestLocation(destLocationPtr);
                        
                        action->setEntity(static_cast<EntityType>(actionObject->entity_type()));
                        action->setPlayerType(static_cast<PlayerType>(actionObject->player()));
                        
                        LocationNode* sourceLocationPtr = _layer->findLocation(actionObject->source_location_name()->str());
                        action->setSourceLocation(sourceLocationPtr);
                        
                        newTrigger->addAction(action);
                        
                        break;
                    }
                    case DeadCreator::ActionBase_PlaySound:
                    {
                        auto actionObject = static_cast<const DeadCreator::PlaySound*>(act->action());
                        auto action = new ActionPlaySound();
                        
                        action->setFileName(actionObject->file_name()->str());
                        
                        newTrigger->addAction(action);
                        
                        break;
                    }
                    case DeadCreator::ActionBase_PlaySoundAtLocation:
                    {
                        auto actionObject = static_cast<const DeadCreator::PlaySoundAtLocation*>(act->action());
                        auto action = new ActionPlaySoundAtLocation();
                        
                        action->setFileName(actionObject->file_name()->str());
                        
                        LocationNode* locationPtr = _layer->findLocation(actionObject->location_name()->str());
                        action->setLocation(locationPtr);
                        
                        newTrigger->addAction(action);
                        
                        break;
                    }
                        
                    default: { cocos2d::log("invalid action type"); break;}
                }
            }
            
            _layer->addTrigger(newTrigger);
        }
        
        _layer->updateCollisionRegion();
        
        
        // menu
        setEnableEditMenu(true);
        setEnableWindowMenu(true);
        
        
        // button
        setEnableSaveButton(true);
        _selectedPlayerType = 0;
        
        changeLayerType(LayerType::TILE);
    }
}


void EditScene::closeGMXLayer()
{
    setEnableEditMenu(false);
    setEnableWindowMenu(false);
    setEnableSaveButton(false);
    
    _layer->removeFromParent();
    _layer = nullptr;
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
    _layerType = static_cast<int>(type);
    
    if ( _layer )
    {
        if ( type == LayerType::LOCATION ) _layer->setVisibleLocations(true);
        else _layer->setVisibleLocations(false);
        
        if ( type == LayerType::TILE ) _layer->setVisibleCollisionRegions(true);
        else _layer->setVisibleCollisionRegions(false);
        
        if ( type != LayerType::ENTITY ) _layer->clearSelectedEntites();
    }
}


bool EditScene::isNew()
{
    return !_showPlayGameLayer;
}


bool EditScene::isOpen()
{
    return _enableOpenMap && !_showPlayGameLayer;
}


bool EditScene::isSave()
{
    return (_enableSaveMap && _isEditMode);
}


bool EditScene::isUndo()
{
    return (_layer &&  _layer->isUndo() && _isEditMode);
}


bool EditScene::isRedo()
{
    return (_layer &&  _layer->isRedo() && _isEditMode);
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


void EditScene::playGame()
{
    _layer->save("temp_game_map");
    
    _playGameLayer->playGame();
    _layer->setVisible(false);
    enableModal(true);
}


void EditScene::stopGame()
{
    _layer->setVisible(true);
    enableModal(false);
    
    try {
        boost::filesystem::remove(boost::filesystem::path("temp_game_map"));
    } catch ( std::exception& e) {
        cocos2d::log("%s", e.what());
    }
}









