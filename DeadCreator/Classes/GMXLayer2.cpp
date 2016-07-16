//
//  GMXLayer2.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 28..
//
//


#include <stack>

#include "GMXLayer2.hpp"
#include "EditScene2.hpp"
#include "TileHelperFunctions.hpp"
#include "PaletteLayer.hpp"
#include "NavigatorLayer.hpp"
#include "HistoryLayer.hpp"
#include "TriggerEditLayer.hpp"
#include "TileToolCommand.hpp"
#include "AddEntityToolCommand.hpp"
#include "RemoveEntityToolCommand.hpp"
#include "EditorSheriff.hpp"
#include "EditorItems.hpp"
#include "CellSpacePartition.hpp"
#include "GameWorld.hpp"
#include "TestScene.hpp"
#include "MainMenu3.hpp"
#include "LocationNode.hpp"
#include "RenameLocationLayer.hpp"
using namespace cocos2d;
using namespace realtrick;

#include "flatbuffers.h"
#include "util.h"
#include "GMXFile_generated.h"

bool GMXLayer2::TITLE_CLICKED = false;

GMXLayer2::GMXLayer2(EditScene2& imguiLayer, GMXFile& file) :
_imguiLayer(imguiLayer),
_file(file),
_worldDebugNode(nullptr),
_localDebugNode(nullptr),
_hoveredTileRegion(nullptr),
_layerSize(Director::getInstance()->getVisibleSize() / 1.5),
_layerPosition(Director::getInstance()->getVisibleSize() / 2 - _layerSize / 2),
_centerViewParam(Vec2::ZERO),
_cameraDirection(Vec2::ZERO),
_camera(nullptr),
_windowSpeed(1000),
_tileRoot(nullptr),
_viewX(30),
_viewY(60)
{}


GMXLayer2::~GMXLayer2()
{
    CC_SAFE_DELETE(_tileToolCommand);
    CC_SAFE_DELETE(_addEntityToolCommand);
    CC_SAFE_DELETE(_removeEntityToolCommand);
    
    _entities.clear();
    
    for(auto& tile : _tiles)
        tile.clear();
    _tiles.clear();
    
    for( auto& tile : _tileImages)
        tile.clear();
    _tileImages.clear();
    
    _selectedEntities.clear();
    
    _currCommand = nullptr;

}


GMXLayer2* GMXLayer2::create(EditScene2& imguiLayer, GMXFile& file)
{
    auto ret = new (std::nothrow) GMXLayer2(imguiLayer, file);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool GMXLayer2::init()
{
    if ( !Layer::init() )
        return false;
    
    
    this->scheduleUpdate();
    
    _imguiLayer.addImGUI([this] {
        
        if ( _isShowWindow ) showWindow();
        
    }, "##GMXLayer");
    
    _clipNode = ClippingRectangleNode::create();
    addChild(_clipNode);
    
    _rootNode = Node::create();
    _clipNode->addChild(_rootNode , 10);
    
    _tileRoot = Node::create();
    _tileRoot->setPosition(_layerSize / 2);
    _clipNode->addChild(_tileRoot);
    
    _worldDebugNode = DrawNode::create();
    _rootNode->addChild(_worldDebugNode);
    
    _selectionRectNode = DrawNode::create();
    _rootNode->addChild(_selectionRectNode, 10);
    
    _localDebugNode = DrawNode::create();
    _clipNode->addChild(_localDebugNode);
    
    _hoveredTileRegion = DrawNode::create();
    _rootNode->addChild(_hoveredTileRegion);
    
    _collisionNode = DrawNode::create();
    _collisionNode->setVisible(false);
    _rootNode->addChild(_collisionNode);
    
    _selectedItem = Sprite::create();
    _rootNode->addChild(_selectedItem, 10);
    
    _cellSpacePartition = EditorCellSpacePartition::create(_file.worldSize, Size(_file.tileWidth * 5, _file.tileHeight * 5));
    addChild(_cellSpacePartition);
    
    _camera = Node::create();
    _camera->setPosition(_layerSize / 2);
    addChild(_camera);
    
    initFile();
    initCollisionData();
    
    _paletteLayer = PaletteLayer::create(*this);
    addChild(_paletteLayer);
    
    _navigatorLayer = NavigatorLayer::create(*this);
    addChild(_navigatorLayer);
    
    _historyLayer = HistoryLayer::create(*this);
    addChild(_historyLayer);
    
    _triggerEditLayer = TriggerEditLayer::create(*this);
    addChild(_triggerEditLayer);
    
    _renameLocationLayer = RenameLocationLayer::create(*this);
    addChild(_renameLocationLayer);
    
    _tileToolCommand = new TileToolCommand(this);
    _addEntityToolCommand = new AddEntityToolCommand(this);
    _removeEntityToolCommand = new RemoveEntityToolCommand(this);
    
    return true;
}


void GMXLayer2::initFile()
{
    int x = _file.numOfTileX + DUMMY_TILE_SIZE * 2;
    int y = _file.numOfTileY * 2 + DUMMY_TILE_SIZE * 4;
    
    _tiles.resize(y);
    for(int i = 0 ; i < y ; ++ i)
    {
        _tiles[i].resize(x);
    }
    
    for(int i = 0 ; i < y; ++ i)
    {
        for(int j = 0 ; j < x ; ++ j)
        {
            Vec2 tilePosition = indexToPosition(j, i, _file.tileWidth, _file.tileHeight, DUMMY_TILE_SIZE);
            _tiles[i][j] = TileBase(j, i, _file.tileInfos[i][j], tilePosition);
        }
    }
    
    _tileImages.resize(_viewY);
    for(int i = 0 ; i < _viewY; ++ i)
    {
        _tileImages[i].resize(_viewX);
    }
    
    for(int i = 0 ; i < _viewY; ++ i)
    {
        for(int j = 0 ; j < _viewX; ++ j)
        {
            _tileImages[i][j] = TileImage::create();
            _tileRoot->addChild(_tileImages[i][j]);
        }
    }
    
    updateChunk(_camera->getPosition());
}


void GMXLayer2::showWindow()
{
    auto& g = *GImGui;
    float height = g.FontBaseSize + g.Style.FramePadding.y * 2.0f;

    ImGui::SetNextWindowSizeConstraints(ImVec2(100, 100),
                                        ImVec2(ImGui::GetIO().DisplaySize.x - WINDOW_PADDING * 4,
                                               ImGui::GetIO().DisplaySize.y - height - ICONBAR_HEIGHT - STATUSBAR_HEIGHT - WINDOW_PADDING * 4));
    ImGui::SetNextWindowPos(ImVec2(_layerPosition.x, _layerPosition.y), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(_layerSize.width, _layerSize.height), ImGuiSetCond_Once);
    
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.8200000, 0.8200000, 0.8200000, 1.0000000));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(200,200));
    ImGui::Begin(_file.fileName.c_str(), &_isShowWindow, ImVec2(0,0), 0.0f,
                 ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoBringToFrontOnFocus |
                 ImGuiWindowFlags_ShowBorders);
    
    auto canvasSize = Size(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
    ImGui::InvisibleButton("##dummy", ImVec2(canvasSize.width, canvasSize.height));
    
    bool positionDirty = false;
    
    if ( _layerPosition.x < WINDOW_PADDING )
    {
        _layerPosition.x = WINDOW_PADDING * 1.5f;
        positionDirty = true;
    }
    else if ( _layerPosition.x + _layerSize.width > g.IO.DisplaySize.x - WINDOW_PADDING )
    {
        _layerPosition.x = g.IO.DisplaySize.x - _layerSize.width - WINDOW_PADDING * 1.5f;
        positionDirty = true;
    }

    if ( _layerPosition.y < height + WINDOW_PADDING + ICONBAR_HEIGHT )
    {
        _layerPosition.y = height + WINDOW_PADDING  * 1.5f + ICONBAR_HEIGHT;
        positionDirty = true;
    }
    
    else if ( _layerPosition.y + _layerSize.height > g.IO.DisplaySize.y - WINDOW_PADDING - STATUSBAR_HEIGHT )
    {
        _layerPosition.y = g.IO.DisplaySize.y - _layerSize.height - WINDOW_PADDING* 1.5f - STATUSBAR_HEIGHT;
        positionDirty = true;
    }
    
    if ( positionDirty ) ImGui::SetWindowPos(ImVec2(_layerPosition.x, _layerPosition.y));
    
    _layerPosition.setPoint(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
    _layerSize.setSize(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
    
    setPosition(_layerPosition.x, Director::getInstance()->getVisibleSize().height - _layerPosition.y - _layerSize.height);
    _tileRoot->setPosition(_layerSize / 2);
    _clipNode->setClippingRegion(cocos2d::Rect(0, 0, _layerSize.width, _layerSize.height));
    
    if ( !_imguiLayer.isModal() ) updateCocosLogic();
    
    ImGui::End();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
    
    if ( _isShowWindow == false )
    {
        setVisible(false);
        _isShowPalette = false;
        _isShowNavigator = false;
        _imguiLayer.setEnableEditMenu(false);
        _imguiLayer.setEnablePlayerMenu(false);
        _imguiLayer.setEnableWindowMenu(false);
        _imguiLayer.setEnableSaveButton(false);
    }
    
    if ( _isShowPalette ) _paletteLayer->showLayer(&_isShowPalette);
    if ( _isShowNavigator ) _navigatorLayer->showLayer(&_isShowNavigator);
    if ( _isShowHistory ) _historyLayer->showLayer(&_isShowHistory);
    if ( _isShowRenameLocationLayer ) _renameLocationLayer->showLayer(&_isShowRenameLocationLayer);
    if ( _isShowTriggerEdit ) _triggerEditLayer->showLayer(&_isShowTriggerEdit);
}


void GMXLayer2::updateCocosLogic()
{
    ImGuiContext& g = *GImGui;
    static float height = g.FontBaseSize + g.Style.FramePadding.y * 2.0f;
    
    // mouse events
    static Vec2 mousePosInCanvas(0, 0);
    mousePosInCanvas = Vec2(ImGui::GetIO().MousePos.x - ImGui::GetCursorScreenPos().x,
                             ImGui::GetContentRegionAvail().y - (ImGui::GetIO().MousePos.y - ImGui::GetCursorScreenPos().y));
    
    _mousePosInWorld = _camera->getPosition() + (Vec2(mousePosInCanvas.x - _layerSize.width / 2 + ImGui::GetStyle().WindowPadding.x,
                                                      mousePosInCanvas.y - _layerSize.height / 2 + ImGui::GetStyle().WindowPadding.y));
    
    _mousePosInWorld.clamp(_camera->getPosition() - Vec2(_layerSize / 2), _camera->getPosition() + Vec2(_layerSize / 2));
    
    static Vec2 mousePosInCocos2dMatrix;
    mousePosInCocos2dMatrix = Vec2(ImGui::GetIO().MousePos.x, ImGui::GetIO().DisplaySize.y - ImGui::GetIO().MousePos.y);
    
    if ( ImGui::IsMouseHoveringWindow() && ImGui::GetIO().MouseClicked[0] )
    {
        cocos2d::Rect boundingBox(_layerPosition.x, ImGui::GetIO().DisplaySize.y - _layerPosition.y - height, _layerSize.width, height);
        if ( boundingBox.containsPoint(mousePosInCocos2dMatrix) )
        {
            GMXLayer2::enableTitleClicked();
        }
    }
    if ( ImGui::GetIO().MouseReleased[0] )
    {
        GMXLayer2::disableTitleClicked();
    }
    
    if ( ImGui::GetIO().MouseClicked[0] && ImGui::IsMouseHoveringWindow() && !GMXLayer2::isTitleClicked() )
    {
        if ( !_isLeftMouseClickEventDone )
        {
            if ( _currCommand ) _currCommand->begin();
            _isLeftMouseClickEventDone = true;
        }
        
    }
    else if ( ImGui::GetIO().MouseReleased[0] && ImGui::IsMouseHoveringWindow() && !GMXLayer2::isTitleClicked() )
    {
        if ( _isLeftMouseClickEventDone )
        {
            _isLeftMouseClickEventDone = false;
            
            if ( _currCommand )
            {
                if ( !_currCommand->empty() )
                {
                    _historyLayer->pushCommand(_currCommand->clone());
                }
                _currCommand->end();
            }
        }
    }
    
    
    // location funcs
    if ( _imguiLayer.getLayerType() == LayerType::LOCATION && ImGui::IsWindowHovered() && !GMXLayer2::isTitleClicked() )
    {
        Vec2 resizeButtonOrigin(_layerPosition.x + _layerSize.width - 30, ImGui::GetIO().DisplaySize.y - _layerPosition.y - _layerSize.height);
        bool isClickedResizeButton = cocos2d::Rect(resizeButtonOrigin.x, resizeButtonOrigin.y, 30, 30).containsPoint(mousePosInCocos2dMatrix);
        if ( !isClickedResizeButton )
        {
            static bool isExistClickableLocation = false;
            if ( ImGui::GetIO().MouseClicked[0] )
            {
                isExistClickableLocation = false;
                std::vector<LocationNode*> locationEntry;
                for( auto& loc : _locations )
                {
                    if ( loc->getAABBBox().containsPoint(_mousePosInWorld) )
                    {
                        isExistClickableLocation = true;
                        locationEntry.push_back(loc);
                    }
                }
                
                if ( isExistClickableLocation )
                {
                    _grabbedLocation = *max_element(std::begin(locationEntry), std::end(locationEntry),
                                                    [] (LocationNode* l1, LocationNode* l2)
                                                    {
                                                        return (l1->getLocationZOrder() < l2->getLocationZOrder());
                                                    });
                    
                    for( auto& loc : _locations )
                    {
                        if ( loc->getLocationName() == _grabbedLocation->getLocationName() ) continue;
                        loc->setSelected(false);
                    }
                    
                    _grabbedLocation->setSelected(true);
                    if ( ImGui::GetIO().MouseDoubleClicked[0] )
                    {
                        _renameLocationLayer->setInputText(_grabbedLocation->getLocationName());
                        _isShowRenameLocationLayer = true;
                        
                        updateChunk(_camera->getPosition());
                        for( auto& loc : _locations )
                        {
                            loc->setSelected(false);
                        }
                    }
                    
                    reorderLocations();
                }
                else
                {
                    for( auto& loc : _locations )
                    {
                        loc->setSelected(false);
                        _grabbedLocation = nullptr;
                    }
                    
                    auto location = LocationNode::create(*this);
                    location->setPositionFromWorldPosition(_mousePosInWorld);
                    location->setLocationZOrder(_locations.size());
                    location->setSelected(true);
                    int number = 0;
                    while ( isOverlappedLocationName("Location" + std::to_string(number)) )
                    {
                        number ++;
                    }
                    location->setLocationName("Location" + std::to_string(number));
                    addLocation(location);
                    
                    _grabbedLocation = location;
                }
            }
            
            if ( ImGui::GetIO().MouseDown[0] )
            {
                static auto oldRectangleIndex = getRectangleTileIndex(_mousePosInWorld, _file.tileWidth, _file.tileHeight);
                auto newRectangleIndex = getRectangleTileIndex(_mousePosInWorld, _file.tileWidth, _file.tileHeight);
                if ( oldRectangleIndex != newRectangleIndex )
                {
                    reorderLocations();
                    oldRectangleIndex = newRectangleIndex;
                }
            }
            
            for ( auto& loc : _locations )
            {
                loc->update(_mousePosInWorld);
            }
        }
    }
    
    if ( _imguiLayer.getLayerType() == LayerType::LOCATION && ImGui::GetIO().MouseReleased[0] )
    {
        for( auto& loc : _locations )
        {
            if ( loc->getLocationName() == _grabbedLocation->getLocationName() ) continue;
            loc->setSelected(false);
        }
    }
    
    _hoveredTileRegion->clear();
    
    int selectedItem = _paletteLayer->getSelectedItem();
    if ( selectedItem == -1 )
    {
        _selectedItem->setTexture("empty_image.png");
    }
    
    if ( ImGui::GetIO().MouseClicked[1] && ImGui::IsMouseHoveringWindow() )
    {
        _paletteLayer->setSelectedItem(-1);
        clearSelectedEntites();
    }
    
    if ( _imguiLayer.getLayerType() == LayerType::ENTITY )
    {
        static bool isSelecting = false;
        if ( selectedItem == - 1)
        {
            if ( (ImGui::IsMouseDragging() || ImGui::GetIO().MouseClicked[0]) && ImGui::IsMouseHoveringWindow() && !GMXLayer2::isTitleClicked() )
            {
                if ( ImGui::GetIO().MouseClicked[0] )
                {
                    _selectRect.origin = _mousePosInWorld;
                }
                _selectRect.size = _mousePosInWorld - _selectRect.origin;
                _selectionRectNode->clear();
                _selectionRectNode->drawRect(_selectRect.origin, _selectRect.origin + Vec2(_selectRect.size), Color4F(0.0, 1.0, 0.0, 0.5));
                
                isSelecting = true;
            }
            
            if ( isSelecting && ImGui::GetIO().MouseReleased[0] )
            {
                Vec2 origin = _selectRect.origin;
                Size dimension = Size(std::abs(_selectRect.size.width), std::abs(_selectRect.size.height));
                if ( _selectRect.size.width < 0 )
                {
                    origin.x = _selectRect.origin.x + _selectRect.size.width;
                }
                if ( _selectRect.size.height < 0 )
                {
                    origin.y = _selectRect.origin.y + _selectRect.size.height;
                }
                
                clearSelectedEntites();
                cocos2d::Rect selectRect(origin, dimension);
                for( auto &ent : _entities )
                {
                    if ( ent.second->isVisible() && selectRect.containsPoint(ent.second->getPosition()) )
                    {
                        ent.second->setSelected(true);
                        _selectedEntities.push_back(ent.second);
                    }
                }
                
                _selectRect = cocos2d::Rect::ZERO;
                _selectionRectNode->clear();
                isSelecting = false;
            }
        }
    }
    
    if ( _imguiLayer.getLayerType() == LayerType::TILE )
    {
        EditorTileType selectedTile = static_cast<EditorTileType>(_paletteLayer->getSelectedItem());
        auto indices = getFocusedTileIndex(_mousePosInWorld, _file.tileWidth, _file.tileHeight, DUMMY_TILE_SIZE);
        if ( selectedTile != EditorTileType::INVALID )
        {
            if ( selectedTile == EditorTileType::DIRT )
            {
                _selectedItem->setTexture("1_1_1234.png");
            }
            else if ( selectedTile == EditorTileType::GRASS )
            {
                _selectedItem->setTexture("selected_grass.png");
            }
            else if ( selectedTile == EditorTileType::WATER )
            {
                _selectedItem->setTexture("3_1_1234.png");
            }
            else if ( selectedTile == EditorTileType::HILL )
            {
                _selectedItem->setTexture("5_1_1234.png");
            }
            
            if ( (ImGui::IsMouseDragging() || ImGui::GetIO().MouseClicked[0]) && ImGui::IsMouseHoveringWindow() && !GMXLayer2::isTitleClicked() )
            {
                Vec2 resizeButtonOrigin(_layerPosition.x + _layerSize.width - 30, ImGui::GetIO().DisplaySize.y - _layerPosition.y - _layerSize.height);
                bool isClickedResizeButton = cocos2d::Rect(resizeButtonOrigin.x, resizeButtonOrigin.y, 30, 30).containsPoint(mousePosInCocos2dMatrix);
                if ( !isClickedResizeButton )
                {
                    putTile(selectedTile, indices.first, indices.second);
                }
            }
        }
        
        Vec2 hoveredRegionCenterPos = _tiles[indices.second][indices.first].getPosition();
        _hoveredTileRegion->drawSegment(hoveredRegionCenterPos + Vec2(-_file.tileWidth / 2, 0), hoveredRegionCenterPos + Vec2(0, _file.tileHeight / 2), 2.0f, Color4F::GREEN);
        _hoveredTileRegion->drawSegment(hoveredRegionCenterPos + Vec2(0, _file.tileHeight / 2), hoveredRegionCenterPos + Vec2(_file.tileWidth / 2, 0), 2.0f, Color4F::GREEN);
        _hoveredTileRegion->drawSegment(hoveredRegionCenterPos + Vec2(_file.tileWidth / 2, 0), hoveredRegionCenterPos + Vec2(0, -_file.tileHeight / 2), 2.0f, Color4F::GREEN);
        _hoveredTileRegion->drawSegment(hoveredRegionCenterPos + Vec2(0, -_file.tileHeight / 2), hoveredRegionCenterPos + Vec2(-_file.tileWidth / 2, 0), 2.0f, Color4F::GREEN);
        _selectedItem->setPosition(_tiles[indices.second][indices.first].getPosition());
        _selectedItem->setOpacity(128);
    }
    else if ( _paletteLayer->getPaletteType() == PaletteType::HUMAN )
    {
        _selectedItem->setPosition(_mousePosInWorld);
        EntityType selectedEntity = static_cast<EntityType>(_paletteLayer->getSelectedItem());
        if ( selectedEntity == EntityType::ENTITY_HUMAN )
        {
            _selectedItem->setSpriteFrame("HumanFistIdleLoop0.png");
            _selectedItem->setOpacity(128);
            if ( (ImGui::GetIO().MouseClicked[0]) && ImGui::IsMouseHoveringWindow() && !GMXLayer2::isTitleClicked() )
            {
                Vec2 resizeButtonOrigin(_layerPosition.x + _layerSize.width - 30, ImGui::GetIO().DisplaySize.y - _layerPosition.y - _layerSize.height);
                bool isClickedResizeButton = cocos2d::Rect(resizeButtonOrigin.x, resizeButtonOrigin.y, 30, 30).containsPoint(mousePosInCocos2dMatrix);
                if ( !isClickedResizeButton )
                {
                    EditorSheriff* ent = EditorSheriff::create(*this, getNextValidID(), cocos2d::ui::Widget::TextureResType::PLIST);
                    ent->setPosition(_mousePosInWorld);
                    ent->setPlayerType(_imguiLayer.getSelectedPlayerType());
                    addEntity(ent, 5);
                }
            }
        }
    }
    else if ( _paletteLayer->getPaletteType() == PaletteType::ITEM )
    {
        _selectedItem->setPosition(_mousePosInWorld);
        EntityType selectedEntity = static_cast<EntityType>(_paletteLayer->getSelectedItem());
        
        if ( selectedEntity == EntityType::BULLET_556MM )
        {
            _selectedItem->setSpriteFrame("5_56mm.png");
            _selectedItem->setOpacity(128);
            if ( (ImGui::GetIO().MouseClicked[0]) && ImGui::IsMouseHoveringWindow() && !GMXLayer2::isTitleClicked() )
            {
                Vec2 resizeButtonOrigin(_layerPosition.x + _layerSize.width - 30, ImGui::GetIO().DisplaySize.y - _layerPosition.y - _layerSize.height);
                bool isClickedResizeButton = cocos2d::Rect(resizeButtonOrigin.x, resizeButtonOrigin.y, 30, 30).containsPoint(mousePosInCocos2dMatrix);
                if ( !isClickedResizeButton )
                {
                    EditorItem556mm* ent = EditorItem556mm::create(*this, getNextValidID(), "5_56mm.png" ,cocos2d::ui::Widget::TextureResType::PLIST);
                    ent->setPosition(_mousePosInWorld);
                    ent->setPlayerType(PlayerType::NEUTRAL);
                    addEntity(ent);
                }
            }
        }
        else if ( selectedEntity == EntityType::BULLET_9MM )
        {
            _selectedItem->setSpriteFrame("9mm.png");
            _selectedItem->setOpacity(128);
            if ( (ImGui::GetIO().MouseClicked[0]) && ImGui::IsMouseHoveringWindow() && !GMXLayer2::isTitleClicked() )
            {
                Vec2 resizeButtonOrigin(_layerPosition.x + _layerSize.width - 30, ImGui::GetIO().DisplaySize.y - _layerPosition.y - _layerSize.height);
                bool isClickedResizeButton = cocos2d::Rect(resizeButtonOrigin.x, resizeButtonOrigin.y, 30, 30).containsPoint(mousePosInCocos2dMatrix);
                if ( !isClickedResizeButton )
                {
                    EditorItem9mm* ent = EditorItem9mm::create(*this, getNextValidID(), "9mm.png" ,cocos2d::ui::Widget::TextureResType::PLIST);
                    ent->setPosition(_mousePosInWorld);
                    ent->setPlayerType(PlayerType::NEUTRAL);
                    addEntity(ent);
                }
            }
        }
        else if ( selectedEntity == EntityType::ITEM_AXE )
        {
            _selectedItem->setSpriteFrame("Axe.png");
            _selectedItem->setOpacity(128);
            if ( (ImGui::GetIO().MouseClicked[0]) && ImGui::IsMouseHoveringWindow() && !GMXLayer2::isTitleClicked() )
            {
                Vec2 resizeButtonOrigin(_layerPosition.x + _layerSize.width - 30, ImGui::GetIO().DisplaySize.y - _layerPosition.y - _layerSize.height);
                bool isClickedResizeButton = cocos2d::Rect(resizeButtonOrigin.x, resizeButtonOrigin.y, 30, 30).containsPoint(mousePosInCocos2dMatrix);
                if ( !isClickedResizeButton )
                {
                    EditorItemAxe* ent = EditorItemAxe::create(*this, getNextValidID(), "Axe.png" ,cocos2d::ui::Widget::TextureResType::PLIST);
                    ent->setPosition(_mousePosInWorld);
                    ent->setPlayerType(PlayerType::NEUTRAL);
                    addEntity(ent);
                }
            }
        }
        else if ( selectedEntity == EntityType::BULLET_SHELL )
        {
            _selectedItem->setSpriteFrame("Shell.png");
            _selectedItem->setOpacity(128);
            if ( (ImGui::GetIO().MouseClicked[0]) && ImGui::IsMouseHoveringWindow() && !GMXLayer2::isTitleClicked() )
            {
                Vec2 resizeButtonOrigin(_layerPosition.x + _layerSize.width - 30, ImGui::GetIO().DisplaySize.y - _layerPosition.y - _layerSize.height);
                bool isClickedResizeButton = cocos2d::Rect(resizeButtonOrigin.x, resizeButtonOrigin.y, 30, 30).containsPoint(mousePosInCocos2dMatrix);
                if ( !isClickedResizeButton )
                {
                    EditorItemShell* ent = EditorItemShell::create(*this, getNextValidID(), "Shell.png" ,cocos2d::ui::Widget::TextureResType::PLIST);
                    ent->setPosition(_mousePosInWorld);
                    ent->setPlayerType(PlayerType::NEUTRAL);
                    addEntity(ent);
                }
            }
        }
        else if ( selectedEntity == EntityType::ITEM_M16A2 )
        {
            _selectedItem->setSpriteFrame("M16A2.png");
            _selectedItem->setOpacity(128);
            if ( (ImGui::GetIO().MouseClicked[0]) && ImGui::IsMouseHoveringWindow() && !GMXLayer2::isTitleClicked() )
            {
                Vec2 resizeButtonOrigin(_layerPosition.x + _layerSize.width - 30, ImGui::GetIO().DisplaySize.y - _layerPosition.y - _layerSize.height);
                bool isClickedResizeButton = cocos2d::Rect(resizeButtonOrigin.x, resizeButtonOrigin.y, 30, 30).containsPoint(mousePosInCocos2dMatrix);
                if ( !isClickedResizeButton )
                {
                    EditorItemM16A2* ent = EditorItemM16A2::create(*this, getNextValidID(), "M16A2.png" ,cocos2d::ui::Widget::TextureResType::PLIST);
                    ent->setPosition(_mousePosInWorld);
                    ent->setPlayerType(PlayerType::NEUTRAL);
                    addEntity(ent);
                }
            }
            
        }
        else if ( selectedEntity == EntityType::ITEM_GLOCK17 )
        {
            _selectedItem->setSpriteFrame("Glock17.png");
            _selectedItem->setOpacity(128);
            if ( (ImGui::GetIO().MouseClicked[0]) && ImGui::IsMouseHoveringWindow() && !GMXLayer2::isTitleClicked() )
            {
                Vec2 resizeButtonOrigin(_layerPosition.x + _layerSize.width - 30, ImGui::GetIO().DisplaySize.y - _layerPosition.y - _layerSize.height);
                bool isClickedResizeButton = cocos2d::Rect(resizeButtonOrigin.x, resizeButtonOrigin.y, 30, 30).containsPoint(mousePosInCocos2dMatrix);
                if ( !isClickedResizeButton )
                {
                    EditorItemGlock17* ent = EditorItemGlock17::create(*this, getNextValidID(), "Glock17.png" ,cocos2d::ui::Widget::TextureResType::PLIST);
                    ent->setPosition(_mousePosInWorld);
                    ent->setPlayerType(PlayerType::NEUTRAL);
                    addEntity(ent);
                }
            }
            
        }
        else if ( selectedEntity == EntityType::ITEM_M1897 )
        {
            _selectedItem->setSpriteFrame("M1897.png");
            _selectedItem->setOpacity(128);
            if ( (ImGui::GetIO().MouseClicked[0]) && ImGui::IsMouseHoveringWindow() && !GMXLayer2::isTitleClicked() )
            {
                Vec2 resizeButtonOrigin(_layerPosition.x + _layerSize.width - 30, ImGui::GetIO().DisplaySize.y - _layerPosition.y - _layerSize.height);
                bool isClickedResizeButton = cocos2d::Rect(resizeButtonOrigin.x, resizeButtonOrigin.y, 30, 30).containsPoint(mousePosInCocos2dMatrix);
                if ( !isClickedResizeButton )
                {
                    EditorItemM1897* ent = EditorItemM1897::create(*this, getNextValidID(), "M1897.png" ,cocos2d::ui::Widget::TextureResType::PLIST);
                    ent->setPosition(_mousePosInWorld);
                    ent->setPlayerType(PlayerType::NEUTRAL);
                    addEntity(ent);
                }
            }
        }
    }
    
    
    if ( !_isShowRenameLocationLayer )
    {
        if ( ImGui::GetIO().KeysDown[262] ) _cameraDirection.x = 1.0f;
        else if ( ImGui::GetIO().KeysDown[263] ) _cameraDirection.x = -1.0f;
        else _cameraDirection.x = 0.0f;
        
        if ( ImGui::GetIO().KeysDown[265] ) _cameraDirection.y = 1.0f;
        else if ( ImGui::GetIO().KeysDown[264] ) _cameraDirection.y = -1.0f;
        else _cameraDirection.y = 0.0f;
        
        auto dt = Director::getInstance()->getDeltaTime();
        _centerViewParam.x += (_cameraDirection.x * _windowSpeed * dt) / (_file.worldSize.width - _layerSize.width);
        _centerViewParam.y += (_cameraDirection.y * _windowSpeed * dt) / (_file.worldSize.height - _layerSize.height);
        _centerViewParam.clamp(Vec2::ZERO, Vec2::ONE);
        
        Vec2 oldPosition = _camera->getPosition();
        Vec2 newPosition = Vec2( _layerSize.width / 2 + (_file.worldSize.width - _layerSize.width) * _centerViewParam.x,
                                _layerSize.height / 2 + (_file.worldSize.height - _layerSize.height) * _centerViewParam.y);
        
        _camera->setPosition(newPosition);
        auto cameraPos = _camera->getPosition();
        cameraPos.clamp(Vec2(_layerSize / 2), Vec2(_file.worldSize) - Vec2(_layerSize / 2));
        _camera->setPosition(cameraPos);
        
        if ( _cellSpacePartition->isUpdateChunk(oldPosition, newPosition) )
        {
            updateChunk(newPosition);
        }
        
        _tileRoot->setPosition( Vec2(_layerSize / 2) + _tileRootWorldPosition - _camera->getPosition() );
        _rootNode->setPosition( -_camera->getPosition() + Vec2(_layerSize / 2) );
        
        if ( ImGui::IsKeyPressed(259) || ImGui::IsKeyPressed(261) ) // back space, delete
        {
            // remove command
            auto prevCommand = _currCommand;
            _currCommand = _removeEntityToolCommand;
            
            _currCommand->begin();
            
            static_cast<RemoveEntityToolCommand*>(_currCommand)->pushEntity(_selectedEntities);
            removeSelectedEntities(true);
            if ( !_currCommand->empty() )
            {
                _historyLayer->pushCommand(_currCommand->clone());
            }
            
            _currCommand->end();
            _currCommand = prevCommand;
            
            
            if ( _imguiLayer.getLayerType() == LayerType::LOCATION )
            {
                // remove location
                for (auto& loc : _locations)
                {
                    loc->setSelected(false);
                }
                removeLocation(_grabbedLocation);
            }
            
        }
        
        if ( ImGui::IsKeyReleased(257) )
        {
            Director::getInstance()->replaceScene(MainMenu3::createScene());
        }
    }
}


void GMXLayer2::setTile(int x, int y, const TileBase& tile, bool isExecCommand)
{
    if ( !isExecCommand )
    {
        static_cast<TileToolCommand*>(_currCommand)->pushTile(_tiles[y][x], tile);
    }
    
    _tiles[y][x] = tile;
    _navigatorLayer->setTile(x, y, tile);
    
    auto rootIndices = getFocusedTileIndex(_tileRootWorldPosition,_file.tileWidth, _file.tileHeight, DUMMY_TILE_SIZE);
    
    auto clickedWorldPosition = indexToPosition(x, y, _file.tileWidth, _file.tileHeight, DUMMY_TILE_SIZE);
    auto clickedIndices = getFocusedTileIndex(clickedWorldPosition, _file.tileWidth, _file.tileHeight, DUMMY_TILE_SIZE);
    
    int offsetX = clickedIndices.first - rootIndices.first;
    int offsetY = clickedIndices.second - rootIndices.second;
    
    int localX = _viewX / 2 + offsetX;
    int localY = _viewY / 2 + offsetY;
    
    if ( localX < 0 || localY < 0 || localX >= _viewX || localY >= _viewY )
        return ;
    
    _tileImages[localY][localX]->setTexture(tile.getNumber() + ".png");
}

void GMXLayer2::updateChunk(const cocos2d::Vec2& pivot)
{
    _tileRootWorldPosition = pivot;
    
    auto index = getFocusedTileIndex(pivot, _file.tileWidth, _file.tileHeight, DUMMY_TILE_SIZE);
    
    index.first -= _viewX / 2;
    index.second -= _viewY / 2;
    
    _worldDebugNode->clear();
    for(int i = 0 ; i < _viewY; ++ i)
    {
        for(int j = 0 ; j < _viewX; ++ j)
        {
            int x = index.first + j;
            int y = index.second + i;
            
            bool viewable = true;
            std::string fileName;
            Vec2 pos;
            Vec2 worldPos;
            if ( x < 0 || x > _file.numOfTileX + DUMMY_TILE_SIZE * 2 - 1 ||
                y < 0 || y > _file.numOfTileY * 2 + DUMMY_TILE_SIZE * 4- 1)
            {
                fileName = "empty_image.png";
                pos = Vec2::ZERO;
                worldPos = Vec2::ZERO;
                viewable = false;
            }
            else
            {
                fileName = _tiles[y][x].getNumber() + ".png";
                pos = _tiles[y][x].getPosition() - _tileRootWorldPosition;
                worldPos = _tiles[y][x].getPosition();
            }
            
            _tileImages[i][j]->setTexture(fileName);
            _tileImages[i][j]->setPosition(pos);
            
            if ( viewable )
            {
                _worldDebugNode->drawLine(Vec2(worldPos.x - _file.tileWidth / 2, worldPos.y),
                                          Vec2(worldPos.x, worldPos.y + _file.tileHeight / 2), Color4F(1, 1, 1, 0.1f));
                
                _worldDebugNode->drawLine(Vec2(worldPos.x, worldPos.y + _file.tileHeight / 2),
                                          Vec2(worldPos.x + _file.tileWidth / 2, worldPos.y), Color4F(1, 1, 1, 0.1f));
                
                _worldDebugNode->drawLine(Vec2(worldPos.x + _file.tileWidth / 2, worldPos.y),
                                          Vec2(worldPos.x, worldPos.y - _file.tileHeight / 2), Color4F(1, 1, 1, 0.1f));
                
                _worldDebugNode->drawLine(Vec2(worldPos.x, worldPos.y - _file.tileHeight / 2),
                                          Vec2(worldPos.x - _file.tileWidth / 2, worldPos.y), Color4F(1, 1, 1, 0.1f));
                
                for(int i = 0 ; i < 4 ; ++ i)
                {
                _worldDebugNode->drawLine(Vec2(worldPos.x - _file.tileWidth / 2 + (_file.tileWidth / 4 * i),
                                               worldPos.y - _file.tileHeight / 2),
                                          Vec2(worldPos.x - _file.tileWidth / 2 + (_file.tileWidth / 4 * i),
                                               worldPos.y + _file.tileHeight / 2),
                                          Color4F(1, 1, 1, 0.05f));
                    
                    _worldDebugNode->drawLine(Vec2(worldPos.x - _file.tileWidth / 2,
                                                   worldPos.y - _file.tileHeight / 2 + (_file.tileHeight / 4 * i)),
                                              Vec2(worldPos.x + _file.tileWidth / 2,
                                                   worldPos.y - _file.tileHeight / 2 + (_file.tileHeight / 4 * i)),
                                              Color4F(1, 1, 1, 0.05f));
                }
            }
        }
    }
    
}


void GMXLayer2::setCenterViewParameter(const cocos2d::Vec2& p)
{
    _centerViewParam = p;
}


void GMXLayer2::putTile(EditorTileType type, int x, int y)
{
    if ( _tiles[y][x].getTileType() == type && _tiles[y][x].getTileTail() == "1234" )
    {
        return ;
    }
    
    const int tileWidth = _file.tileWidth;
    const int tileHeight = _file.tileHeight;
    const int dummy = DUMMY_TILE_SIZE;
    
    std::stack<TileBase> s;
    
    TileBase init = TileBase(x, y, TileBase::getTileHeader(type) + "1234", indexToPosition(x, y, tileWidth, tileHeight, dummy));
    s.push(init);
    while( !s.empty() )
    {
        TileBase temp = s.top();
        s.pop();
        
        std::string number = temp.getNumber();
        int xx = temp.getIndexX();
        int yy = temp.getIndexY();
        
        if ( temp.getNumber().at(0) != '1')
        {
            // 추가하는 로직
            if ( TileBase::getTileTail(temp.getNumber()) == "1234" )
            {
                auto nei = getNeighborTiles(xx, yy);
                s.push(TileBase(nei[0].first, nei[0].second, TileBase::getTileHeader(number) + "2", indexToPosition(nei[0].first, nei[0].second, tileWidth, tileHeight, dummy)));
                s.push(TileBase(nei[1].first, nei[1].second, TileBase::getTileHeader(number) + "23", indexToPosition(nei[1].first, nei[1].second, tileWidth, tileHeight, dummy)));
                s.push(TileBase(nei[2].first, nei[2].second, TileBase::getTileHeader(number) + "3", indexToPosition(nei[2].first, nei[2].second, tileWidth, tileHeight, dummy)));
                s.push(TileBase(nei[3].first, nei[3].second, TileBase::getTileHeader(number) + "34", indexToPosition(nei[3].first, nei[3].second, tileWidth, tileHeight, dummy)));
                s.push(TileBase(nei[4].first, nei[4].second, TileBase::getTileHeader(number) + "4", indexToPosition(nei[4].first, nei[4].second, tileWidth, tileHeight, dummy)));
                s.push(TileBase(nei[5].first, nei[5].second, TileBase::getTileHeader(number) + "14", indexToPosition(nei[5].first, nei[5].second, tileWidth, tileHeight, dummy)));
                s.push(TileBase(nei[6].first, nei[6].second, TileBase::getTileHeader(number) + "1", indexToPosition(nei[6].first, nei[6].second, tileWidth, tileHeight, dummy)));
                s.push(TileBase(nei[7].first, nei[7].second, TileBase::getTileHeader(number) + "12", indexToPosition(nei[7].first, nei[7].second, tileWidth, tileHeight, dummy)));
                
                setTile(xx, yy, temp);
            }
            else
            {
                if ( _tiles[yy][xx].getNumber().at(0) == number[0] )
                {
                    // 같은 타일의 경우 합친후 적용한다.
                    std::string originalTail = _tiles[yy][xx].getTileTail();
                    std::string tail = temp.getTileTail();
                    
                    originalTail += tail;
                    std::sort(begin(originalTail), end(originalTail));
                    originalTail.erase(unique(begin(originalTail), end(originalTail)), end(originalTail));
                    
                    std::string finalTile = _tiles[yy][xx].getTileHeader() + originalTail;
                    
                    temp.setNumber(finalTile);
                    setTile(xx, yy, temp);
                    
                }
                else if ( _tiles[yy][xx].getNumber().at(0) == '1' )
                {
                    // 흙 타일일 경우에는 바로 적용한다.
                    setTile(xx, yy, temp);
                }
                else
                {
                    // 다른 타일일 경우
                    
                    auto nei = getNeighborTiles(xx, yy);
                    std::string tail = temp.getTileTail();
                    if ( tail == "2")
                    {
                        s.push(TileBase(nei[0].first, nei[0].second, TileBase::getTileHeader(EditorTileType::DIRT) + "2", indexToPosition(nei[0].first, nei[0].second, tileWidth, tileHeight, dummy)));
                        s.push(TileBase(nei[1].first, nei[1].second, TileBase::getTileHeader(EditorTileType::DIRT) + "23", indexToPosition(nei[1].first, nei[1].second, tileWidth, tileHeight, dummy)));
                        s.push(TileBase(nei[7].first, nei[7].second, TileBase::getTileHeader(EditorTileType::DIRT) + "12", indexToPosition(nei[7].first, nei[7].second, tileWidth, tileHeight, dummy)));
                    }
                    else if ( tail == "23")
                    {
                        s.push(TileBase(nei[1].first, nei[1].second, TileBase::getTileHeader(EditorTileType::DIRT) + "23", indexToPosition(nei[1].first, nei[1].second, tileWidth, tileHeight, dummy)));
                    }
                    else if ( tail == "3")
                    {
                        s.push(TileBase(nei[1].first, nei[1].second, TileBase::getTileHeader(EditorTileType::DIRT) + "23", indexToPosition(nei[1].first, nei[1].second, tileWidth, tileHeight, dummy)));
                        s.push(TileBase(nei[2].first, nei[2].second, TileBase::getTileHeader(EditorTileType::DIRT) + "3", indexToPosition(nei[2].first, nei[2].second, tileWidth, tileHeight, dummy)));
                        s.push(TileBase(nei[3].first, nei[3].second, TileBase::getTileHeader(EditorTileType::DIRT) + "34", indexToPosition(nei[3].first, nei[3].second, tileWidth, tileHeight, dummy)));
                    }
                    else if ( tail == "34")
                    {
                        s.push(TileBase(nei[3].first, nei[3].second, TileBase::getTileHeader(EditorTileType::DIRT) + "34", indexToPosition(nei[3].first, nei[3].second, tileWidth, tileHeight, dummy)));
                    }
                    else if ( tail == "4")
                    {
                        s.push(TileBase(nei[3].first, nei[3].second, TileBase::getTileHeader(EditorTileType::DIRT) + "34", indexToPosition(nei[3].first, nei[3].second, tileWidth, tileHeight, dummy)));
                        s.push(TileBase(nei[4].first, nei[4].second, TileBase::getTileHeader(EditorTileType::DIRT) + "4", indexToPosition(nei[4].first, nei[4].second, tileWidth, tileHeight, dummy)));
                        s.push(TileBase(nei[5].first, nei[5].second, TileBase::getTileHeader(EditorTileType::DIRT) + "14", indexToPosition(nei[5].first, nei[5].second, tileWidth, tileHeight, dummy)));
                    }
                    else if ( tail == "14")
                    {
                        s.push(TileBase(nei[5].first, nei[5].second, TileBase::getTileHeader(EditorTileType::DIRT) + "14", indexToPosition(nei[5].first, nei[5].second, tileWidth, tileHeight, dummy)));
                    }
                    else if ( tail == "1")
                    {
                        s.push(TileBase(nei[5].first, nei[5].second, TileBase::getTileHeader(EditorTileType::DIRT) + "14", indexToPosition(nei[5].first, nei[5].second, tileWidth, tileHeight, dummy)));
                        s.push(TileBase(nei[6].first, nei[6].second, TileBase::getTileHeader(EditorTileType::DIRT) + "1", indexToPosition(nei[6].first, nei[6].second, tileWidth, tileHeight, dummy)));
                        s.push(TileBase(nei[7].first, nei[7].second, TileBase::getTileHeader(EditorTileType::DIRT) + "12", indexToPosition(nei[7].first, nei[7].second, tileWidth, tileHeight, dummy)));
                    }
                    else if ( tail == "12")
                    {
                        s.push(TileBase(nei[7].first, nei[7].second, TileBase::getTileHeader(EditorTileType::DIRT) + "12", indexToPosition(nei[7].first, nei[7].second, tileWidth, tileHeight, dummy)));
                    }
                    
                    setTile(xx, yy, temp);
                }
            }
        }
        else // if ( temp.tileNumber[0] == '1' )
        {
            // 지우는 로직
            if ( temp.getTileTail() == "1234" )
            {
                auto nei = getNeighborTiles(xx, yy);
                s.push(TileBase(nei[0].first, nei[0].second, temp.getTileHeader() + "2", indexToPosition(nei[0].first, nei[0].second, tileWidth, tileHeight, dummy)));
                s.push(TileBase(nei[1].first, nei[1].second, temp.getTileHeader() + "23", indexToPosition(nei[1].first, nei[1].second, tileWidth, tileHeight, dummy)));
                s.push(TileBase(nei[2].first, nei[2].second, temp.getTileHeader() + "3", indexToPosition(nei[2].first, nei[2].second, tileWidth, tileHeight, dummy)));
                s.push(TileBase(nei[3].first, nei[3].second, temp.getTileHeader() + "34", indexToPosition(nei[3].first, nei[3].second, tileWidth, tileHeight, dummy)));
                s.push(TileBase(nei[4].first, nei[4].second, temp.getTileHeader() + "4", indexToPosition(nei[4].first, nei[4].second, tileWidth, tileHeight, dummy)));
                s.push(TileBase(nei[5].first, nei[5].second, temp.getTileHeader() + "14", indexToPosition(nei[5].first, nei[5].second, tileWidth, tileHeight, dummy)));
                s.push(TileBase(nei[6].first, nei[6].second, temp.getTileHeader() + "1", indexToPosition(nei[6].first, nei[6].second, tileWidth, tileHeight, dummy)));
                s.push(TileBase(nei[7].first, nei[7].second, temp.getTileHeader() + "12", indexToPosition(nei[7].first, nei[7].second, tileWidth, tileHeight, dummy)));
                
                setTile(xx, yy, temp);
            }
            else
            {
                if ( _tiles[yy][xx].getNumber().at(0) == '1')
                {
                    continue;
                }
                
                std::string originalTail = _tiles[yy][xx].getTileTail();
                std::string tail = temp.getTileTail();
                
                for(auto& d : tail)
                {
                    originalTail.erase(std::remove(begin(originalTail), end(originalTail), d), end(originalTail));
                }
                
                std::string finalTile;
                if ( originalTail.empty() ) finalTile = TileBase::getTileHeader(EditorTileType::DIRT) + "1234";
                else finalTile = _tiles[yy][xx].getTileHeader() + originalTail;
                
                temp.setNumber(finalTile);
                setTile(xx, yy, temp);
            }
        }
    }
    
    updateCollisionRegion();
    
}


bool GMXLayer2::addEntity(EditorEntityBase* entity, int localZOrder, bool isExecCommand)
{
    clearSelectedEntites();
    
    auto iter = _entities.find(entity->getID());
    if ( iter == std::end(_entities))
    {
        // todo
        //if ( ... aabb intersection )
        if ( !isExecCommand )
        {
            _navigatorLayer->addEntity(entity);
            _rootNode->addChild(entity, localZOrder);
            _entities.insert( {entity->getID(), entity} );
            static_cast<AddEntityToolCommand*>(_currCommand)->pushEntity(entity);
        }
        
        return true;
    }
    
    if ( isExecCommand )
    {
        iter->second->setVisible(true);
        _navigatorLayer->addEntity(entity);
        return true;
    }
    
    return false;
}


bool GMXLayer2::addEntityForce(EditorEntityBase* entity, int localZOrder)
{
    auto iter = _entities.find(entity->getID());
    if ( iter == std::end(_entities))
    {
        _navigatorLayer->addEntity(entity);
        _rootNode->addChild(entity, localZOrder);
        _entities.insert( {entity->getID(), entity} );
        
        return true;
    }
    
    return false;
}


bool GMXLayer2::eraseEntity(int id, bool isExecCommand)
{
    auto iter = _entities.find(id);
    if ( iter != std::end(_entities))
    {
        _navigatorLayer->eraseEntity(id);
        iter->second->setVisible(false);
        iter->second->setSelected(false);
        if ( !isExecCommand)
        {
            _entities.erase(id);
            iter->second->removeFromParent();
        }
        
        return true;
    }
    
    return false;
}


void GMXLayer2::setCommand(CommandBase* newCommand)
{
    _currCommand = newCommand;
    
    if ( dynamic_cast<TileToolCommand*>(newCommand) )
    {
        _imguiLayer.changeLayerType(LayerType::TILE);
    }
    else if ( dynamic_cast<AddEntityToolCommand*>(newCommand) )
    {
        _imguiLayer.changeLayerType(LayerType::ENTITY);
    }
    else if ( dynamic_cast<RemoveEntityToolCommand*>(newCommand) )
    {
        // ... //
    }
}


void GMXLayer2::removeSelectedEntities(bool isExecCommand)
{
    for ( auto& ent : _selectedEntities )
    {
        eraseEntity(ent->getID(), isExecCommand);
    }
    clearSelectedEntites();
}


void GMXLayer2::clearSelectedEntites()
{
    for (auto& ent : _selectedEntities )
    {
        if ( ent ) ent->setSelected(false);
    }
    _selectedEntities.clear();
}


void GMXLayer2::initCollisionData()
{
    _tileCollisions["5_1_1_LD"].push_back(Vec2(88, 25));
    _tileCollisions["5_1_1_LD"].push_back(Vec2(83, 30));
    _tileCollisions["5_1_1_LD"].push_back(Vec2(77, 39));
    _tileCollisions["5_1_1_LD"].push_back(Vec2(68, 36));
    _tileCollisions["5_1_1_LD"].push_back(Vec2(61, 39));
    _tileCollisions["5_1_1_LD"].push_back(Vec2(47, 34));
    _tileCollisions["5_1_1_LD"].push_back(Vec2(44, 27));
    
    _tileCollisions["5_1_2_LU"].push_back(Vec2(106, 84));
    _tileCollisions["5_1_2_LU"].push_back(Vec2(101, 82));
    _tileCollisions["5_1_2_LU"].push_back(Vec2(94, 70));
    _tileCollisions["5_1_2_LU"].push_back(Vec2(101, 42));
    
    _tileCollisions["5_1_3_RU"].push_back(Vec2(39, 102));
    _tileCollisions["5_1_3_RU"].push_back(Vec2(48, 89));
    _tileCollisions["5_1_3_RU"].push_back(Vec2(56, 86));
    _tileCollisions["5_1_3_RU"].push_back(Vec2(67, 88));
    _tileCollisions["5_1_3_RU"].push_back(Vec2(71, 92));
    _tileCollisions["5_1_3_RU"].push_back(Vec2(79, 89));
    _tileCollisions["5_1_3_RU"].push_back(Vec2(88, 96));
    
    _tileCollisions["5_1_4_RD"].push_back(Vec2(25, 39));
    _tileCollisions["5_1_4_RD"].push_back(Vec2(36, 57));
    _tileCollisions["5_1_4_RD"].push_back(Vec2(28, 64));
    _tileCollisions["5_1_4_RD"].push_back(Vec2(33, 77));
    _tileCollisions["5_1_4_RD"].push_back(Vec2(29, 85));
    
    _tileCollisions["5_1_12_LU"].push_back(Vec2(103, 88));
    _tileCollisions["5_1_12_LU"].push_back(Vec2(95, 84));
    _tileCollisions["5_1_12_LU"].push_back(Vec2(87, 72));
    _tileCollisions["5_1_12_LU"].push_back(Vec2(69, 66));
    _tileCollisions["5_1_12_LU"].push_back(Vec2(66, 44));
    _tileCollisions["5_1_12_LU"].push_back(Vec2(44, 27));
    
    _tileCollisions["5_1_13_LD"].push_back(Vec2(88, 26));
    _tileCollisions["5_1_13_LD"].push_back(Vec2(87, 41));
    _tileCollisions["5_1_13_LD"].push_back(Vec2(91, 50));
    _tileCollisions["5_1_13_LD"].push_back(Vec2(90, 62));
    _tileCollisions["5_1_13_LD"].push_back(Vec2(43, 62));
    _tileCollisions["5_1_13_LD"].push_back(Vec2(46, 52));
    _tileCollisions["5_1_13_LD"].push_back(Vec2(44, 41));
    _tileCollisions["5_1_13_LD"].push_back(Vec2(41, 36));
    _tileCollisions["5_1_13_LD"].push_back(Vec2(41, 28));
    
    _tileCollisions["5_1_13_RU"].push_back(Vec2(37, 101));
    _tileCollisions["5_1_13_RU"].push_back(Vec2(43, 97));
    _tileCollisions["5_1_13_RU"].push_back(Vec2(41, 68));
    _tileCollisions["5_1_13_RU"].push_back(Vec2(43, 62));
    _tileCollisions["5_1_13_RU"].push_back(Vec2(90, 62));
    _tileCollisions["5_1_13_RU"].push_back(Vec2(91, 71));
    _tileCollisions["5_1_13_RU"].push_back(Vec2(82, 86));
    _tileCollisions["5_1_13_RU"].push_back(Vec2(88, 97));
    
    _tileCollisions["5_1_14_LD"].push_back(Vec2(87, 24));
    _tileCollisions["5_1_14_LD"].push_back(Vec2(66, 49));
    _tileCollisions["5_1_14_LD"].push_back(Vec2(61, 51));
    _tileCollisions["5_1_14_LD"].push_back(Vec2(55, 58));
    _tileCollisions["5_1_14_LD"].push_back(Vec2(52, 58));
    _tileCollisions["5_1_14_LD"].push_back(Vec2(40, 77));
    _tileCollisions["5_1_14_LD"].push_back(Vec2(26, 81));
    _tileCollisions["5_1_14_LD"].push_back(Vec2(27, 84));
    
    _tileCollisions["5_1_23_RU"].push_back(Vec2(39, 102));
    _tileCollisions["5_1_23_RU"].push_back(Vec2(62, 75));
    _tileCollisions["5_1_23_RU"].push_back(Vec2(79, 67));
    _tileCollisions["5_1_23_RU"].push_back(Vec2(91, 47));
    _tileCollisions["5_1_23_RU"].push_back(Vec2(98, 43));
    
    _tileCollisions["5_1_24_RD"].push_back(Vec2(24, 39));
    _tileCollisions["5_1_24_RD"].push_back(Vec2(38, 35));
    _tileCollisions["5_1_24_RD"].push_back(Vec2(56, 37));
    _tileCollisions["5_1_24_RD"].push_back(Vec2(63, 41));
    _tileCollisions["5_1_24_RD"].push_back(Vec2(64, 55));
    _tileCollisions["5_1_24_RD"].push_back(Vec2(57, 62));
    _tileCollisions["5_1_24_RD"].push_back(Vec2(59, 69));
    _tileCollisions["5_1_24_RD"].push_back(Vec2(54, 79));
    _tileCollisions["5_1_24_RD"].push_back(Vec2(46, 82));
    _tileCollisions["5_1_24_RD"].push_back(Vec2(39, 77));
    _tileCollisions["5_1_24_RD"].push_back(Vec2(28, 82));
    
    _tileCollisions["5_1_24_LU"].push_back(Vec2(107, 84));
    _tileCollisions["5_1_24_LU"].push_back(Vec2(90, 89));
    _tileCollisions["5_1_24_LU"].push_back(Vec2(78, 81));
    _tileCollisions["5_1_24_LU"].push_back(Vec2(74, 68));
    _tileCollisions["5_1_24_LU"].push_back(Vec2(81, 42));
    _tileCollisions["5_1_24_LU"].push_back(Vec2(91, 38));
    _tileCollisions["5_1_24_LU"].push_back(Vec2(98, 38));
    
    _tileCollisions["5_1_34_RD"].push_back(Vec2(26, 37));
    _tileCollisions["5_1_34_RD"].push_back(Vec2(37, 44));
    _tileCollisions["5_1_34_RD"].push_back(Vec2(43, 54));
    _tileCollisions["5_1_34_RD"].push_back(Vec2(61, 62));
    _tileCollisions["5_1_34_RD"].push_back(Vec2(72, 80));
    _tileCollisions["5_1_34_RD"].push_back(Vec2(72, 90));
    _tileCollisions["5_1_34_RD"].push_back(Vec2(86, 97));
    
    _tileCollisions["5_1_123_RU"].push_back(Vec2(37, 101));
    _tileCollisions["5_1_123_RU"].push_back(Vec2(49, 94));
    _tileCollisions["5_1_123_RU"].push_back(Vec2(63, 75));
    _tileCollisions["5_1_123_RU"].push_back(Vec2(70, 71));
    _tileCollisions["5_1_123_RU"].push_back(Vec2(66, 44));
    _tileCollisions["5_1_123_RU"].push_back(Vec2(44, 27));
    
    _tileCollisions["5_1_124_LU"].push_back(Vec2(103, 87));
    _tileCollisions["5_1_124_LU"].push_back(Vec2(87, 70));
    _tileCollisions["5_1_124_LU"].push_back(Vec2(69, 66));
    _tileCollisions["5_1_124_LU"].push_back(Vec2(66, 48));
    _tileCollisions["5_1_124_LU"].push_back(Vec2(57, 57));
    _tileCollisions["5_1_124_LU"].push_back(Vec2(51, 57));
    _tileCollisions["5_1_124_LU"].push_back(Vec2(38, 77));
    _tileCollisions["5_1_124_LU"].push_back(Vec2(25, 81));
    
    _tileCollisions["5_1_134_LD"].push_back(Vec2(86, 23));
    _tileCollisions["5_1_134_LD"].push_back(Vec2(56, 57));
    _tileCollisions["5_1_134_LD"].push_back(Vec2(73, 85));
    _tileCollisions["5_1_134_LD"].push_back(Vec2(72, 89));
    _tileCollisions["5_1_134_LD"].push_back(Vec2(86, 97));
    
    _tileCollisions["5_1_234_RD"].push_back(Vec2(26, 37));
    _tileCollisions["5_1_234_RD"].push_back(Vec2(44, 55));
    _tileCollisions["5_1_234_RD"].push_back(Vec2(57, 58));
    _tileCollisions["5_1_234_RD"].push_back(Vec2(68, 72));
    _tileCollisions["5_1_234_RD"].push_back(Vec2(81, 65));
    _tileCollisions["5_1_234_RD"].push_back(Vec2(88, 49));
    _tileCollisions["5_1_234_RD"].push_back(Vec2(98, 44));
    
    // seed 2
    _tileCollisions["5_2_1_LD"].push_back(Vec2(89, 26));
    _tileCollisions["5_2_1_LD"].push_back(Vec2(71, 37));
    _tileCollisions["5_2_1_LD"].push_back(Vec2(64, 28));
    _tileCollisions["5_2_1_LD"].push_back(Vec2(50, 33));
    
    _tileCollisions["5_2_2_LU"].push_back(Vec2(105, 86));
    _tileCollisions["5_2_2_LU"].push_back(Vec2(90, 78));
    _tileCollisions["5_2_2_LU"].push_back(Vec2(92, 51));
    
    _tileCollisions["5_2_3_RU"].push_back(Vec2(38, 103));
    _tileCollisions["5_2_3_RU"].push_back(Vec2(58, 95));
    
    _tileCollisions["5_2_4_RD"].push_back(Vec2(25, 40));
    _tileCollisions["5_2_4_RD"].push_back(Vec2(42, 53));
    _tileCollisions["5_2_4_RD"].push_back(Vec2(44, 59));
    _tileCollisions["5_2_4_RD"].push_back(Vec2(36, 72));
    _tileCollisions["5_2_4_RD"].push_back(Vec2(40, 80));
    
    _tileCollisions["5_2_12_LU"].push_back(Vec2(104, 87));
    _tileCollisions["5_2_12_LU"].push_back(Vec2(81, 71));
    _tileCollisions["5_2_12_LU"].push_back(Vec2(65, 48));
    _tileCollisions["5_2_12_LU"].push_back(Vec2(51, 41));
    
    _tileCollisions["5_2_13_LD"].push_back(Vec2(89, 25));
    _tileCollisions["5_2_13_LD"].push_back(Vec2(87, 40));
    _tileCollisions["5_2_13_LD"].push_back(Vec2(91, 51));
    _tileCollisions["5_2_13_LD"].push_back(Vec2(88, 60));
    _tileCollisions["5_2_13_LD"].push_back(Vec2(75, 59));
    _tileCollisions["5_2_13_LD"].push_back(Vec2(67, 52));
    _tileCollisions["5_2_13_LD"].push_back(Vec2(52, 53));
    _tileCollisions["5_2_13_LD"].push_back(Vec2(45, 48));
    _tileCollisions["5_2_13_LD"].push_back(Vec2(41, 34));
    
    _tileCollisions["5_2_13_RU"].push_back(Vec2(39, 102));
    _tileCollisions["5_2_13_RU"].push_back(Vec2(44, 96));
    _tileCollisions["5_2_13_RU"].push_back(Vec2(40, 67));
    _tileCollisions["5_2_13_RU"].push_back(Vec2(47, 59));
    _tileCollisions["5_2_13_RU"].push_back(Vec2(66, 57));
    _tileCollisions["5_2_13_RU"].push_back(Vec2(81, 68));
    _tileCollisions["5_2_13_RU"].push_back(Vec2(86, 82));
    _tileCollisions["5_2_13_RU"].push_back(Vec2(81, 87));
    
    _tileCollisions["5_2_14_LD"].push_back(Vec2(90, 27));
    _tileCollisions["5_2_14_LD"].push_back(Vec2(87, 35));
    _tileCollisions["5_2_14_LD"].push_back(Vec2(74, 45));
    _tileCollisions["5_2_14_LD"].push_back(Vec2(67, 61));
    _tileCollisions["5_2_14_LD"].push_back(Vec2(38, 74));
    _tileCollisions["5_2_14_LD"].push_back(Vec2(32, 86));
    
    _tileCollisions["5_2_23_RU"].push_back(Vec2(39, 102));
    _tileCollisions["5_2_23_RU"].push_back(Vec2(44, 94));
    _tileCollisions["5_2_23_RU"].push_back(Vec2(57, 86));
    _tileCollisions["5_2_23_RU"].push_back(Vec2(56, 81));
    _tileCollisions["5_2_23_RU"].push_back(Vec2(61, 77));
    _tileCollisions["5_2_23_RU"].push_back(Vec2(61, 70));
    _tileCollisions["5_2_23_RU"].push_back(Vec2(79, 67));
    _tileCollisions["5_2_23_RU"].push_back(Vec2(88, 62));
    _tileCollisions["5_2_23_RU"].push_back(Vec2(97, 47));
    
    _tileCollisions["5_2_24_RD"].push_back(Vec2(25, 40));
    _tileCollisions["5_2_24_RD"].push_back(Vec2(47, 31));
    _tileCollisions["5_2_24_RD"].push_back(Vec2(58, 26));
    _tileCollisions["5_2_24_RD"].push_back(Vec2(68, 36));
    _tileCollisions["5_2_24_RD"].push_back(Vec2(70, 51));
    _tileCollisions["5_2_24_RD"].push_back(Vec2(75, 57));
    _tileCollisions["5_2_24_RD"].push_back(Vec2(72, 70));
    _tileCollisions["5_2_24_RD"].push_back(Vec2(64, 74));
    _tileCollisions["5_2_24_RD"].push_back(Vec2(52, 85));
    _tileCollisions["5_2_24_RD"].push_back(Vec2(47, 91));
    _tileCollisions["5_2_24_RD"].push_back(Vec2(32, 90));
    
    _tileCollisions["5_2_24_LU"].push_back(Vec2(105, 86));
    _tileCollisions["5_2_24_LU"].push_back(Vec2(84, 86));
    _tileCollisions["5_2_24_LU"].push_back(Vec2(76, 72));
    _tileCollisions["5_2_24_LU"].push_back(Vec2(79, 53));
    _tileCollisions["5_2_24_LU"].push_back(Vec2(85, 40));
    
    _tileCollisions["5_2_34_RD"].push_back(Vec2(26, 38));
    _tileCollisions["5_2_34_RD"].push_back(Vec2(31, 42));
    _tileCollisions["5_2_34_RD"].push_back(Vec2(33, 49));
    _tileCollisions["5_2_34_RD"].push_back(Vec2(53, 62));
    _tileCollisions["5_2_34_RD"].push_back(Vec2(63, 80));
    _tileCollisions["5_2_34_RD"].push_back(Vec2(77, 88));
    
    _tileCollisions["5_2_123_RU"].push_back(Vec2(39, 103));
    _tileCollisions["5_2_123_RU"].push_back(Vec2(46, 93));
    _tileCollisions["5_2_123_RU"].push_back(Vec2(57, 85));
    _tileCollisions["5_2_123_RU"].push_back(Vec2(56, 81));
    _tileCollisions["5_2_123_RU"].push_back(Vec2(61, 77));
    _tileCollisions["5_2_123_RU"].push_back(Vec2(61, 69));
    _tileCollisions["5_2_123_RU"].push_back(Vec2(79, 66));
    _tileCollisions["5_2_123_RU"].push_back(Vec2(70, 56));
    _tileCollisions["5_2_123_RU"].push_back(Vec2(67, 49));
    _tileCollisions["5_2_123_RU"].push_back(Vec2(52, 42));
    _tileCollisions["5_2_123_RU"].push_back(Vec2(43, 27));
    
    _tileCollisions["5_2_124_LU"].push_back(Vec2(103, 88));
    _tileCollisions["5_2_124_LU"].push_back(Vec2(70, 56));
    _tileCollisions["5_2_124_LU"].push_back(Vec2(43, 73));
    _tileCollisions["5_2_124_LU"].push_back(Vec2(38, 72));
    _tileCollisions["5_2_124_LU"].push_back(Vec2(31, 86));
    
    _tileCollisions["5_2_134_LD"].push_back(Vec2(90, 27));
    _tileCollisions["5_2_134_LD"].push_back(Vec2(74, 45));
    _tileCollisions["5_2_134_LD"].push_back(Vec2(66, 62));
    _tileCollisions["5_2_134_LD"].push_back(Vec2(60, 64));
    _tileCollisions["5_2_134_LD"].push_back(Vec2(57, 71));
    _tileCollisions["5_2_134_LD"].push_back(Vec2(65, 83));
    _tileCollisions["5_2_134_LD"].push_back(Vec2(82, 91));
    _tileCollisions["5_2_134_LD"].push_back(Vec2(88, 98));
    
    _tileCollisions["5_2_234_RD"].push_back(Vec2(26, 37));
    _tileCollisions["5_2_234_RD"].push_back(Vec2(38, 45));
    _tileCollisions["5_2_234_RD"].push_back(Vec2(38, 53));
    _tileCollisions["5_2_234_RD"].push_back(Vec2(51, 61));
    _tileCollisions["5_2_234_RD"].push_back(Vec2(61, 77));
    _tileCollisions["5_2_234_RD"].push_back(Vec2(63, 72));
    _tileCollisions["5_2_234_RD"].push_back(Vec2(70, 68));
    _tileCollisions["5_2_234_RD"].push_back(Vec2(89, 62));
    _tileCollisions["5_2_234_RD"].push_back(Vec2(98, 48));
    _tileCollisions["5_2_234_RD"].push_back(Vec2(97, 43));
    
    // seed 3
    _tileCollisions["5_3_1_LD"].push_back(Vec2(88, 26));
    _tileCollisions["5_3_1_LD"].push_back(Vec2(76, 39));
    _tileCollisions["5_3_1_LD"].push_back(Vec2(68, 36));
    _tileCollisions["5_3_1_LD"].push_back(Vec2(60, 39));
    _tileCollisions["5_3_1_LD"].push_back(Vec2(45, 30));
    
    
    _tileCollisions["5_3_2_LU"].push_back(Vec2(107, 85));
    _tileCollisions["5_3_2_LU"].push_back(Vec2(99, 84));
    _tileCollisions["5_3_2_LU"].push_back(Vec2(95, 78));
    _tileCollisions["5_3_2_LU"].push_back(Vec2(101, 66));
    _tileCollisions["5_3_2_LU"].push_back(Vec2(100, 61));
    _tileCollisions["5_3_2_LU"].push_back(Vec2(91, 55));
    _tileCollisions["5_3_2_LU"].push_back(Vec2(99, 41));
    
    _tileCollisions["5_3_3_RU"].push_back(Vec2(38, 102));
    _tileCollisions["5_3_3_RU"].push_back(Vec2(53, 91));
    _tileCollisions["5_3_3_RU"].push_back(Vec2(63, 88));
    _tileCollisions["5_3_3_RU"].push_back(Vec2(72, 91));
    _tileCollisions["5_3_3_RU"].push_back(Vec2(77, 88));
    _tileCollisions["5_3_3_RU"].push_back(Vec2(84, 91));
    
    _tileCollisions["5_3_4_RD"].push_back(Vec2(24, 40));
    _tileCollisions["5_3_4_RD"].push_back(Vec2(33, 69));
    
    _tileCollisions["5_3_12_LU"].push_back(Vec2(103, 88));
    _tileCollisions["5_3_12_LU"].push_back(Vec2(100, 86));
    _tileCollisions["5_3_12_LU"].push_back(Vec2(93, 86));
    _tileCollisions["5_3_12_LU"].push_back(Vec2(84, 73));
    _tileCollisions["5_3_12_LU"].push_back(Vec2(78, 74));
    _tileCollisions["5_3_12_LU"].push_back(Vec2(66, 66));
    _tileCollisions["5_3_12_LU"].push_back(Vec2(56, 49));
    _tileCollisions["5_3_12_LU"].push_back(Vec2(54, 37));
    _tileCollisions["5_3_12_LU"].push_back(Vec2(47, 36));
    _tileCollisions["5_3_12_LU"].push_back(Vec2(43, 31));
    
    _tileCollisions["5_3_13_LD"].push_back(Vec2(88, 24));
    _tileCollisions["5_3_13_LD"].push_back(Vec2(88, 48));
    _tileCollisions["5_3_13_LD"].push_back(Vec2(77, 60));
    _tileCollisions["5_3_13_LD"].push_back(Vec2(61, 60));
    _tileCollisions["5_3_13_LD"].push_back(Vec2(56, 56));
    _tileCollisions["5_3_13_LD"].push_back(Vec2(50, 59));
    _tileCollisions["5_3_13_LD"].push_back(Vec2(40, 53));
    _tileCollisions["5_3_13_LD"].push_back(Vec2(41, 35));
    _tileCollisions["5_3_13_LD"].push_back(Vec2(44, 31));
    
    _tileCollisions["5_3_13_RU"].push_back(Vec2(38, 102));
    _tileCollisions["5_3_13_RU"].push_back(Vec2(42, 92));
    _tileCollisions["5_3_13_RU"].push_back(Vec2(45, 72));
    _tileCollisions["5_3_13_RU"].push_back(Vec2(50, 66));
    _tileCollisions["5_3_13_RU"].push_back(Vec2(57, 63));
    _tileCollisions["5_3_13_RU"].push_back(Vec2(69, 67));
    _tileCollisions["5_3_13_RU"].push_back(Vec2(78, 65));
    _tileCollisions["5_3_13_RU"].push_back(Vec2(89, 79));
    
    _tileCollisions["5_3_14_LD"].push_back(Vec2(89, 26));
    _tileCollisions["5_3_14_LD"].push_back(Vec2(65, 54));
    _tileCollisions["5_3_14_LD"].push_back(Vec2(50, 61));
    _tileCollisions["5_3_14_LD"].push_back(Vec2(39, 80));
    
    _tileCollisions["5_3_23_RU"].push_back(Vec2(41, 103));
    _tileCollisions["5_3_23_RU"].push_back(Vec2(59, 80));
    _tileCollisions["5_3_23_RU"].push_back(Vec2(72, 70));
    _tileCollisions["5_3_23_RU"].push_back(Vec2(81, 63));
    _tileCollisions["5_3_23_RU"].push_back(Vec2(84, 52));
    _tileCollisions["5_3_23_RU"].push_back(Vec2(99, 47));
    _tileCollisions["5_3_23_RU"].push_back(Vec2(99, 42));
    
    _tileCollisions["5_3_24_RD"].push_back(Vec2(25, 41));
    _tileCollisions["5_3_24_RD"].push_back(Vec2(31, 40));
    _tileCollisions["5_3_24_RD"].push_back(Vec2(38, 37));
    _tileCollisions["5_3_24_RD"].push_back(Vec2(55, 38));
    _tileCollisions["5_3_24_RD"].push_back(Vec2(65, 43));
    _tileCollisions["5_3_24_RD"].push_back(Vec2(66, 62));
    _tileCollisions["5_3_24_RD"].push_back(Vec2(62, 66));
    _tileCollisions["5_3_24_RD"].push_back(Vec2(65, 77));
    _tileCollisions["5_3_24_RD"].push_back(Vec2(49, 90));
    _tileCollisions["5_3_24_RD"].push_back(Vec2(33, 91));
    
    _tileCollisions["5_3_24_LU"].push_back(Vec2(105, 87));
    _tileCollisions["5_3_24_LU"].push_back(Vec2(91, 90));
    _tileCollisions["5_3_24_LU"].push_back(Vec2(81, 88));
    _tileCollisions["5_3_24_LU"].push_back(Vec2(68, 76));
    _tileCollisions["5_3_24_LU"].push_back(Vec2(66, 68));
    _tileCollisions["5_3_24_LU"].push_back(Vec2(72, 57));
    _tileCollisions["5_3_24_LU"].push_back(Vec2(70, 48));
    _tileCollisions["5_3_24_LU"].push_back(Vec2(75, 41));
    _tileCollisions["5_3_24_LU"].push_back(Vec2(85, 42));
    _tileCollisions["5_3_24_LU"].push_back(Vec2(91, 37));
    
    _tileCollisions["5_3_34_RD"].push_back(Vec2(27, 37));
    _tileCollisions["5_3_34_RD"].push_back(Vec2(40, 56));
    _tileCollisions["5_3_34_RD"].push_back(Vec2(47, 57));
    _tileCollisions["5_3_34_RD"].push_back(Vec2(53, 62));
    _tileCollisions["5_3_34_RD"].push_back(Vec2(60, 61));
    _tileCollisions["5_3_34_RD"].push_back(Vec2(61, 80));
    _tileCollisions["5_3_34_RD"].push_back(Vec2(68, 91));
    _tileCollisions["5_3_34_RD"].push_back(Vec2(85, 99));
    
    _tileCollisions["5_3_123_RU"].push_back(Vec2(40, 104));
    _tileCollisions["5_3_123_RU"].push_back(Vec2(60, 81));
    _tileCollisions["5_3_123_RU"].push_back(Vec2(71, 73));
    _tileCollisions["5_3_123_RU"].push_back(Vec2(54, 45));
    _tileCollisions["5_3_123_RU"].push_back(Vec2(54, 37));
    _tileCollisions["5_3_123_RU"].push_back(Vec2(47, 35));
    _tileCollisions["5_3_123_RU"].push_back(Vec2(41, 31));
    
    _tileCollisions["5_3_124_LU"].push_back(Vec2(103, 89));
    _tileCollisions["5_3_124_LU"].push_back(Vec2(100, 86));
    _tileCollisions["5_3_124_LU"].push_back(Vec2(92, 86));
    _tileCollisions["5_3_124_LU"].push_back(Vec2(82, 73));
    _tileCollisions["5_3_124_LU"].push_back(Vec2(71, 71));
    _tileCollisions["5_3_124_LU"].push_back(Vec2(65, 66));
    _tileCollisions["5_3_124_LU"].push_back(Vec2(61, 57));
    _tileCollisions["5_3_124_LU"].push_back(Vec2(49, 62));
    _tileCollisions["5_3_124_LU"].push_back(Vec2(37, 82));
    _tileCollisions["5_3_124_LU"].push_back(Vec2(30, 84));
    
    _tileCollisions["5_3_134_LD"].push_back(Vec2(89, 25));
    _tileCollisions["5_3_134_LD"].push_back(Vec2(65, 54));
    _tileCollisions["5_3_134_LD"].push_back(Vec2(56, 59));
    _tileCollisions["5_3_134_LD"].push_back(Vec2(60, 63));
    _tileCollisions["5_3_134_LD"].push_back(Vec2(61, 80));
    _tileCollisions["5_3_134_LD"].push_back(Vec2(67, 90));
    _tileCollisions["5_3_134_LD"].push_back(Vec2(86, 100));
    
    _tileCollisions["5_3_234_RD"].push_back(Vec2(27, 37));
    _tileCollisions["5_3_234_RD"].push_back(Vec2(41, 57));
    _tileCollisions["5_3_234_RD"].push_back(Vec2(47, 56));
    _tileCollisions["5_3_234_RD"].push_back(Vec2(51, 61));
    _tileCollisions["5_3_234_RD"].push_back(Vec2(60, 61));
    _tileCollisions["5_3_234_RD"].push_back(Vec2(61, 78));
    _tileCollisions["5_3_234_RD"].push_back(Vec2(72, 70));
    _tileCollisions["5_3_234_RD"].push_back(Vec2(84, 55));
    _tileCollisions["5_3_234_RD"].push_back(Vec2(90, 49));
    _tileCollisions["5_3_234_RD"].push_back(Vec2(101, 47));
}


void GMXLayer2::updateCollisionRegion()
{
    for (int i = 0 ; i < _file.numOfTileY * 2 + DUMMY_TILE_SIZE * 4; ++ i)
    {
        for(int j = 0 ; j < _file.numOfTileX + DUMMY_TILE_SIZE * 2; ++ j)
        {
            if (_tiles[i][j].getTileType() == EditorTileType::HILL)
            {
                if ( _tiles[i][j].getTileTail() != "1234" ) _tiles[i][j].setInputState(TileInputState::VALID);
                else _tiles[i][j].setInputState(TileInputState::INVALID);
            }
            else
            {
                _tiles[i][j].setInputState(TileInputState::INVALID);
            }
        }
    }
    
    _collisionRegions.clear();
    for (int i = 0 ; i < _file.numOfTileY * 2 + DUMMY_TILE_SIZE * 4; ++ i)
    {
        for(int j = 0 ; j < _file.numOfTileX + DUMMY_TILE_SIZE * 2; ++ j)
        {
            if ( _tiles[i][j].getInputState() != TileInputState::INVALID )
            {
                std::vector<Vec2> polygon;
                int x = j; int y = i;
                
                std::string tail = _tiles[y][x].getTileTail();
                std::string in = getInDirection(x, y);
                std::string key = _tiles[y][x].getNumber() + in;
                
                bool isSpecial = false;
                if ( tail == "13" )
                {
                    TileInputState inputState = _tiles[y][x].getInputState();
                    if ( inputState == TileInputState::VALID ) _tiles[y][x].setInputState( TileInputState::RIGHT_UP );
                    else if ( inputState == TileInputState::LEFT_DOWN || inputState == TileInputState::RIGHT_UP ) _tiles[y][x].setInputState( TileInputState::INVALID );
                    isSpecial = true;
                }
                else if ( tail == "24" )
                {
                    TileInputState inputState = _tiles[y][x].getInputState();
                    if ( inputState == TileInputState::VALID ) _tiles[y][x].setInputState( TileInputState::RIGHT_DOWN );
                    else if ( inputState == TileInputState::LEFT_UP || inputState == TileInputState::RIGHT_DOWN ) _tiles[y][x].setInputState( TileInputState::INVALID );
                    isSpecial = true;
                }
            
                if ( !isSpecial )
                {
                    _tiles[y][x].setInputState(TileInputState::INVALID);
                }
                
                auto collisions = _tileCollisions[key];
                for( auto& vert : collisions )
                {
                    polygon.push_back(_tiles[y][x].getPosition() - Vec2(_file.tileWidth / 2, _file.tileHeight / 2) + Vec2(vert.x, _file.tileHeight - vert.y));
                }
                
                Vec2 startPoint = collisions.front();
                auto startIndex = std::make_pair(x, y);
                
                while ( true )
                {
                    std::string prevTailWithInput = tail + in;
                    x = getNextTileIndex(prevTailWithInput, x, y).first;
                    y = getNextTileIndex(prevTailWithInput, x, y).second;
                    
                    tail = _tiles[y][x].getTileTail();
                    in = getOutDirection(prevTailWithInput);
                    key = _tiles[y][x].getNumber() + in;
                    
                    auto collisions = _tileCollisions[key];
                    if ( startIndex == std::make_pair(x, y) && startPoint.equals(collisions.front()) )
                    {
                        break;
                    }
                    
                    for( auto& vert : collisions )
                    {
                        polygon.push_back(_tiles[y][x].getPosition() - Vec2(_file.tileWidth / 2, _file.tileHeight / 2) + Vec2(vert.x, _file.tileHeight - vert.y));
                    }

                    bool isSpecial = false;
                    if ( tail == "13" )
                    {
                        TileInputState inputState = _tiles[y][x].getInputState();
                        if ( inputState == TileInputState::VALID && in == "_RU" ) _tiles[y][x].setInputState(TileInputState::LEFT_DOWN);
                        else if ( inputState == TileInputState::RIGHT_UP && in == "_RU" ) _tiles[y][x].setInputState(TileInputState::INVALID);
                        else if ( inputState == TileInputState::VALID && in == "_LD" ) _tiles[y][x].setInputState(TileInputState::RIGHT_UP);
                        else if ( inputState == TileInputState::LEFT_DOWN && in == "_LD" ) _tiles[y][x].setInputState(TileInputState::INVALID);
                        isSpecial = true;
                    }
                    else if ( tail == "24" )
                    {
                        TileInputState inputState = _tiles[y][x].getInputState();
                        if ( inputState == TileInputState::VALID && in == "_LU" ) _tiles[y][x].setInputState(TileInputState::RIGHT_DOWN);
                        else if ( inputState == TileInputState::LEFT_UP && in == "_LU") _tiles[y][x].setInputState(TileInputState::INVALID);
                        else if ( inputState == TileInputState::VALID && in == "_RD" ) _tiles[y][x].setInputState(TileInputState::LEFT_UP);
                        else if ( inputState == TileInputState::RIGHT_DOWN && in == "_RD") _tiles[y][x].setInputState(TileInputState::INVALID);
                        
                        isSpecial = true;
                    }
            
                    if ( !isSpecial )
                    {
                        _tiles[y][x].setInputState( TileInputState::INVALID );
                    }
                }
                
                _collisionRegions.push_back(polygon);
            }
        }
    }
    
    _collisionNode->clear();
    for(auto& poly : _collisionRegions)
    {
        for(int i = 0 ; i < poly.size()-1; ++ i)
        {
            _collisionNode->drawSegment(poly[i], poly[i+1], 1.0f, Color4F(1.0f, 1.0f, 1.0f, 0.5f));
        }
        _collisionNode->drawSegment(poly.back(), poly.front(), 1.0f, Color4F(1.0f, 1.0f, 1.0f, 0.5f));
    }
}


std::string GMXLayer2::getInDirection(int x, int y)
{
    std::string tail = _tiles[y][x].getTileTail();
    
    if (tail == "1") return "_LD";
    else if (tail == "2") return "_LU";
    else if (tail == "3") return "_RU";
    else if (tail == "4") return "_RD";
    else if (tail == "12") return "_LU";
    else if (tail == "14") return "_LD";
    else if (tail == "23") return "_RD";
    else if (tail == "34") return "_RD";
    else if (tail == "123") return "_RU";
    else if (tail == "124") return "_LU";
    else if (tail == "134") return "_LD";
    else if (tail == "234") return "_RD";
    else if (tail == "13")
    {
        if (_tiles[y][x].getInputState() == TileInputState::VALID) return "_LD";
        else if (_tiles[y][x].getInputState() == TileInputState::LEFT_DOWN) return "_LD";
        else if (_tiles[y][x].getInputState() == TileInputState::RIGHT_UP) return "_RU";
    }
    else if (tail == "24")
    {
        if (_tiles[y][x].getInputState() == TileInputState::VALID) return "_RD";
        else if (_tiles[y][x].getInputState() == TileInputState::LEFT_UP) return "_LU";
        else if (_tiles[y][x].getInputState() == TileInputState::RIGHT_DOWN) return "_RD";
    }
    
    return "#INVALID INPUT";
}


std::string GMXLayer2::getOutDirection(const std::string& tail)
{
    if ( tail == "1_LD" ) return "_LU";
    else if ( tail == "2_LU" ) return "_RU";
    else if ( tail == "3_RU" ) return "_RD";
    else if ( tail == "4_RD" ) return "_LD";
    
    else if ( tail == "12_LU" ) return "_LU";
    else if ( tail == "14_LD" ) return "_LD";
    else if (tail == "23_RU") return "_RU";
    else if (tail == "34_RD") return "_RD";
    
    else if (tail == "123_RU") return "_LU";
    else if (tail == "124_LU") return "_LD";
    else if (tail == "134_LD") return "_RD";
    else if (tail == "234_RD") return "_RU";
    
    else if (tail == "13_LD") return "_LU";
    else if (tail == "13_RU") return "_RD";
    else if ( tail == "24_LU") return "_RU";
    else if ( tail == "24_RD") return "_LD";
    
    return "#INVALID INPUT";
}


std::pair<int, int> GMXLayer2::getNextTileIndex(const std::string& tailWithInputDir, int x, int y)
{
    enum { LEFT_DOWN = 0, LEFT_UP = 1, RIGHT_DOWN = 2, RIGHT_UP = 3 };
    int nextDir;
    
    if ( tailWithInputDir == "1_LD" ) nextDir = LEFT_UP;
    else if ( tailWithInputDir == "2_LU" ) nextDir = RIGHT_UP;
    else if ( tailWithInputDir == "3_RU" ) nextDir = RIGHT_DOWN;
    else if ( tailWithInputDir == "4_RD" ) nextDir = LEFT_DOWN;
    
    else if ( tailWithInputDir == "12_LU" ) nextDir = LEFT_UP;
    else if ( tailWithInputDir == "13_LD" ) nextDir = LEFT_UP;
    else if ( tailWithInputDir == "13_RU" ) nextDir = RIGHT_DOWN;
    else if ( tailWithInputDir == "14_LD" ) nextDir =  LEFT_DOWN ;
    else if ( tailWithInputDir == "23_RU" ) nextDir =  RIGHT_UP;
    else if ( tailWithInputDir == "24_RD" ) nextDir = LEFT_DOWN;
    else if ( tailWithInputDir == "24_LU" ) nextDir = RIGHT_UP;
    else if ( tailWithInputDir == "34_RD" ) nextDir = RIGHT_DOWN;
    else if ( tailWithInputDir == "123_RU" ) nextDir = LEFT_UP;
    else if ( tailWithInputDir == "124_LU" ) nextDir = LEFT_DOWN;
    
    else if ( tailWithInputDir == "134_LD" ) nextDir = RIGHT_DOWN;
    else if ( tailWithInputDir == "234_RD" ) nextDir = RIGHT_UP;
    else std::runtime_error("invalid tile tail.");
    
    if (y % 2 == 0)
    {
        std::pair<int, int> table[4];
        table[0] = std::make_pair(-1, -1);
        table[1] = std::make_pair(-1, 1);
        table[2] = std::make_pair(0, -1);
        table[3] = std::make_pair(0, 1);
        
        auto dxdy = table[nextDir];
        return std::make_pair(x + dxdy.first, y + dxdy.second);
    }
    else
    {
        std::pair<int, int> table[4];
        table[0] = std::make_pair(0, -1);
        table[1] = std::make_pair(0, 1);
        table[2] = std::make_pair(1, -1);
        table[3] = std::make_pair(1, 1);
        
        auto dxdy = table[nextDir];
        return std::make_pair(x + dxdy.first, y + dxdy.second);
    }
}


void GMXLayer2::save(const std::string& path)
{
    _isFirstFile = false;
    _currFilePath = path;
    
    log("save! path: %s", path.c_str());
    log("file name: %s", _file.fileName.c_str());
    log("default tile: %d", _file.defaultTile);
    log("number of tile x: %d", _file.numOfTileX);
    log("number of tile y: %d", _file.numOfTileY);
    log("tile width: %d", _file.tileWidth);
    log("tile height: %d", _file.tileHeight);
    log("world size: %.f, %.f", _file.worldSize.width, _file.worldSize.height);
    
    for (int i = 0 ; i < _file.numOfTileY * 2 + DUMMY_TILE_SIZE * 4; ++ i)
    {
        for(int j = 0 ; j < _file.numOfTileX + DUMMY_TILE_SIZE * 2; ++ j)
        {
            if ( _tiles[i][j].getTileType() != static_cast<EditorTileType>(_file.defaultTile) )
            {
                log("[%d, %d]: %s", i, j, _tiles[i][j].getNumber().c_str());
            }
        }
    }
    
    flatbuffers::FlatBufferBuilder builder;
    
    // tile infos
    std::vector<flatbuffers::Offset<DeadCreator::TileInfo>> tileInfos;
    for (int i = 0 ; i < _file.numOfTileY * 2 + DUMMY_TILE_SIZE * 4; ++ i)
    {
        for(int j = 0 ; j < _file.numOfTileX + DUMMY_TILE_SIZE * 2; ++ j)
        {
            if ( _tiles[i][j].getTileType() != static_cast<EditorTileType>(_file.defaultTile) )
            {
                DeadCreator::Coord coord(j ,i);
                auto tile = DeadCreator::CreateTileInfo(builder, builder.CreateString(_tiles[i][j].getNumber()), &coord);
                tileInfos.push_back(tile);
            }
        }
    }
    
    DeadCreator::Coord numOfTiles(_file.numOfTileX, _file.numOfTileY);
    DeadCreator::Size tileSize(_file.tileWidth, _file.tileHeight);
    
    // collision regions
    std::vector<flatbuffers::Offset<DeadCreator::Polygon>> collisionRegions;
    for(int i = 0 ; i < _collisionRegions.size() ; ++ i)
    {
        std::vector<DeadCreator::Vector2> polygon;
        for(int j = 0; j < _collisionRegions[i].size(); ++ j)
        {
            polygon.push_back(DeadCreator::Vector2(_collisionRegions[i][j].x, _collisionRegions[i][j].y));
        }
        auto poly = DeadCreator::CreatePolygon(builder, builder.CreateVectorOfStructs(polygon));
        collisionRegions.push_back(poly);
    }
    
    // entities
    std::vector<flatbuffers::Offset<DeadCreator::Entity>> entities;
    for( auto& ent : _entities )
    {
        if ( ent.second->isVisible() )
        {
            DeadCreator::Vector2 v(ent.second->getPosition().x, ent.second->getPosition().y);
            auto e = DeadCreator::CreateEntity(builder,
                                               static_cast<int>(ent.second->getPlayerType()),
                                               static_cast<int>(ent.second->getEntityType()),
                                               &v);
            entities.push_back(e);
        }
    }
    
    DeadCreator::Size cellSpaceSize(_file.tileWidth * 5, _file.tileHeight * 5);
    
    auto file = DeadCreator::CreateGMXFile(builder,
                                           static_cast<DeadCreator::TileType>(_file.defaultTile),
                                           builder.CreateVector(tileInfos), &numOfTiles, &tileSize,
                                           builder.CreateVector(collisionRegions),
                                           builder.CreateVector(entities),
                                           &cellSpaceSize);
    builder.Finish(file);
    flatbuffers::SaveFile(path.c_str(),
                          reinterpret_cast<const char*>(builder.GetBufferPointer()),
                          builder.GetSize(),
                          true);
}


bool GMXLayer2::addLocation(LocationNode* node)
{
    _locations.push_back(node);
    _rootNode->addChild(node, 20);
    return true;
}


bool GMXLayer2::removeLocation(LocationNode* node)
{
    auto iter = std::find(_locations.begin(), _locations.end(), node);
    if ( iter != _locations.end() )
    {
        _locations.erase(iter);
        node->removeFromParent();
        node = nullptr;
        return true;
    }
    return false;
}


void GMXLayer2::setVisibleLocations(bool visible)
{
    for (auto& loc : _locations)
    {
        loc->setVisible(visible);
        loc->update(Vec2::ZERO);
    }
}


void GMXLayer2::reorderLocations()
{
    std::vector<LocationNode*> overlappedLocations;
    for( auto& loc : _locations )
    {
        if ( loc->getLocationName() == _grabbedLocation->getLocationName() ) continue;
        if ( _grabbedLocation->getAABBBox().intersectsRect(loc->getAABBBox()) )
        {
            overlappedLocations.push_back(loc);
        }
    }
    
    if ( !overlappedLocations.empty() )
    {
        auto maxZorderNode = *std::max_element(std::begin(overlappedLocations), std::end(overlappedLocations),
                                               [](LocationNode* l1, LocationNode* l2)
                                               {
                                                   return l1->getLocationZOrder() < l2->getLocationZOrder();
                                               });
        
        _grabbedLocation->setLocationZOrder(maxZorderNode->getLocationZOrder() + 1);
        
        std::sort(std::begin(_locations), std::end(_locations), [](LocationNode* l1, LocationNode* l2) {
            return (l1->getLocationZOrder() < l2->getLocationZOrder());
        });
    }
    
    else
    {
        _grabbedLocation->setLocationZOrder(0);
    }
}


bool GMXLayer2::isOverlappedLocationName(const std::string& name) const
{
    for(auto& loc : _locations)
    {
        if (loc->getLocationName() == name) return true;
    }
    return false;
}




