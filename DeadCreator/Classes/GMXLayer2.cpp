//
//  GMXLayer2.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 28..
//
//


#include "GMXLayer2.hpp"
#include "GMXFile.hpp"
#include "TileBase.hpp"
#include "PaletteLayer.hpp"
#include "NavigatorLayer.hpp"
#include "EditScene2.hpp"
#include "TileHelperFunctions.hpp"
#include "SizeProtocol.h"
using namespace cocos2d;

GMXLayer2::GMXLayer2(EditScene2& imguiLayer, GMXFile& file) :
_imguiLayer(imguiLayer),
_file(file),
_worldDebugNode(nullptr),
_localDebugNode(nullptr),
_hoveredTileRegion(nullptr),
_visibleSize(Director::getInstance()->getVisibleSize()),
_canvasSize(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y),
_layerSize(Director::getInstance()->getVisibleSize() / 1.5),
_layerPosition(Director::getInstance()->getVisibleSize() / 2 - _layerSize / 2),
_centerViewParam(Vec2::ZERO),
_cameraDirection(Vec2::ZERO),
_camera(nullptr),
_windowSpeed(1000),
_tileRoot(nullptr),
_viewX(15),
_viewY(30)
{}


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
    
    for(int i = 0 ; i < 256 ; ++ i)
    {
        _isKeyDown[i] = false;
    }
    
    auto keylistener = EventListenerKeyboard::create();
    keylistener->onKeyPressed = CC_CALLBACK_2(GMXLayer2::onKeyPressed, this);
    keylistener->onKeyReleased = CC_CALLBACK_2(GMXLayer2::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);
    
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
    
    _localDebugNode = DrawNode::create();
    _clipNode->addChild(_localDebugNode);
    
    _hoveredTileRegion = DrawNode::create();
    _rootNode->addChild(_hoveredTileRegion);
    
    _cellSpacePartition = CellSpacePartition::create(_file.worldSize, Size(500, 500));
    addChild(_cellSpacePartition);
    
    _camera = Camera2D::create();
    _camera->setPosition(_layerSize / 2);
    addChild(_camera);
    
    initFile();
    
    _paletteLayer = PaletteLayer::create(_imguiLayer);
    addChild(_paletteLayer);
    
    _navigatorLayer = NavigatorLayer::create(_imguiLayer, *this);
    addChild(_navigatorLayer);
    
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
            Vec2 tilePosition;
            if ( i % 2 == 0 )
            {
                tilePosition.setPoint(j * _file.tileWidth - (_file.tileWidth * DUMMY_TILE_SIZE),
                                      i * _file.tileHeight / 2 - (_file.tileHeight * DUMMY_TILE_SIZE));
            }
            else
            {
                tilePosition.setPoint(_file.tileWidth / 2 + j * _file.tileWidth - (_file.tileWidth * DUMMY_TILE_SIZE),
                                      i * _file.tileHeight / 2 - (_file.tileHeight * DUMMY_TILE_SIZE));
            }
            
            _tiles[i][j] = new TileBase( _file.tileInfos[i][j], tilePosition);
        }
    }
    
    _tileImages.resize(_viewY);
    _tileIndices.resize(_viewY);
    for(int i = 0 ; i < _viewY; ++ i)
    {
        _tileImages[i].resize(_viewX);
        _tileIndices[i].resize(_viewX);
    }
    
    for(int i = 0 ; i < _viewY; ++ i)
    {
        for(int j = 0 ; j < _viewX; ++ j)
        {
            _tileImages[i][j] = TileImage::create();
            _tileRoot->addChild(_tileImages[i][j]);
            
            _tileIndices[i][j] = ui::Text::create("", "", 20);
            _tileIndices[i][j]->setOpacity(128);
            _tileRoot->addChild(_tileIndices[i][j]);
        }
    }
    
    updateChunk(_camera->getPosition());
}


void GMXLayer2::showWindow()
{
    ImGuiState& g = *GImGui;
    float height = g.FontBaseSize + g.Style.FramePadding.y * 2.0f;
    
    if ( _layerSize.width + _layerPosition.x > g.IO.DisplaySize.x - SizeProtocol::WINDOW_PADDING )
        _layerSize.width = g.IO.DisplaySize.x - SizeProtocol::WINDOW_PADDING - _layerPosition.x;
    
    if ( _layerSize.height + _layerPosition.y > g.IO.DisplaySize.y - SizeProtocol::WINDOW_PADDING - height )
        _layerSize.height = g.IO.DisplaySize.y - SizeProtocol::WINDOW_PADDING - _layerPosition.y - height;

    
    if ( _layerPosition.x < SizeProtocol::WINDOW_PADDING )
        _layerPosition.x = SizeProtocol::WINDOW_PADDING;
    
    if ( _layerPosition.y < height + SizeProtocol::WINDOW_PADDING )
        _layerPosition.y = height + SizeProtocol::WINDOW_PADDING;
    
    if ( _layerPosition.x + _layerSize.width > g.IO.DisplaySize.x - SizeProtocol::WINDOW_PADDING )
        _layerPosition.x = g.IO.DisplaySize.x - _layerSize.width - SizeProtocol::WINDOW_PADDING;
    
    if ( _layerPosition.y + _layerSize.height > g.IO.DisplaySize.y - SizeProtocol::WINDOW_PADDING - SizeProtocol::STATUSBAR_HEIGHT )
        _layerPosition.y = g.IO.DisplaySize.y - _layerSize.height - SizeProtocol::WINDOW_PADDING - SizeProtocol::STATUSBAR_HEIGHT;
    
    
    ImGui::SetNextWindowPos(ImVec2(_layerPosition.x, _layerPosition.y), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(_layerSize.width, _layerSize.height), ImGuiSetCond_Always);
    
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.8200000, 0.8200000, 0.8200000, 1.0000000));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(200,200));
    ImGui::Begin(_file.fileName.c_str(), &_isShowWindow, ImVec2(0,0), 0.0f,
                 ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoBringToFrontOnFocus |
                 ImGuiWindowFlags_ShowBorders);
  
    ImGui::InvisibleButton("##dummy", ImGui::GetContentRegionAvail());
    
    _layerPosition.setPoint(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
    _layerSize.setSize(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
    
    _canvasSize = Size(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    _mousePosInCanvas = Vec2(ImGui::GetIO().MousePos.x - canvasPos.x,
                            _canvasSize.height - (ImGui::GetIO().MousePos.y - canvasPos.y));
    
    _mousePosInWorld = _camera->getPosition() + (Vec2(_mousePosInCanvas.x - _layerSize.width / 2 + ImGui::GetStyle().WindowPadding.x,
                                                      _mousePosInCanvas.y - _layerSize.height / 2 + ImGui::GetStyle().WindowPadding.y));
    
    _mousePosInWorld.clamp(_camera->getPosition() - Vec2(_layerSize / 2), _camera->getPosition() + Vec2(_layerSize / 2));
 
    ImGui::End();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    setPosition(_layerPosition.x, _visibleSize.height - _layerPosition.y - _layerSize.height);
    _tileRoot->setPosition(_layerSize / 2);
    
    _clipNode->setClippingRegion(Rect(0, 0, _layerSize.width, _layerSize.height));
    
    _localDebugNode->clear();
    _localDebugNode->drawDot(Vec2(_layerSize / 2), 5.0f, Color4F::YELLOW);
    
    auto indices = getFocusedTileIndex(_mousePosInWorld, _file.tileWidth, _file.tileHeight, DUMMY_TILE_SIZE);
    
    _hoveredTileRegion->clear();
    
    Vec2 hoveredRegionCenterPos = _tiles[indices.second][indices.first]->getPosition();
    _hoveredTileRegion->drawSegment(hoveredRegionCenterPos + Vec2(-_file.tileWidth / 2, 0), hoveredRegionCenterPos + Vec2(0, _file.tileHeight / 2), 2.0f, Color4F::GREEN);
    _hoveredTileRegion->drawSegment(hoveredRegionCenterPos + Vec2(0, _file.tileHeight / 2), hoveredRegionCenterPos + Vec2(_file.tileWidth / 2, 0), 2.0f, Color4F::GREEN);
    _hoveredTileRegion->drawSegment(hoveredRegionCenterPos + Vec2(_file.tileWidth / 2, 0), hoveredRegionCenterPos + Vec2(0, -_file.tileHeight / 2), 2.0f, Color4F::GREEN);
    _hoveredTileRegion->drawSegment(hoveredRegionCenterPos + Vec2(0, -_file.tileHeight / 2), hoveredRegionCenterPos + Vec2(-_file.tileWidth / 2, 0), 2.0f, Color4F::GREEN);
    
    _centerViewParam = Vec2((_camera->getPosition().x - _layerSize.width / 2) / (_file.worldSize.width - _layerSize.width),
                            (_camera->getPosition().y - _layerSize.height / 2) / (_file.worldSize.height - _layerSize.height));
    
    if ( _isShowWindow == false )
    {
        setVisible(false);
        _isShowPalette = false;
        _isShowNavigator = false;
        _imguiLayer.setEnableEditMenu(false);
        _imguiLayer.setEnablePlayerMenu(false);
        _imguiLayer.setEnableWindowMenu(false);
    }
    
    if ( _isShowPalette ) _paletteLayer->showLayer(&_isShowPalette);
    if ( _isShowNavigator ) _navigatorLayer->showLayer(&_isShowNavigator);
}



void GMXLayer2::setTile(int x, int y, TileBase* tile)
{
    _tileImages[y][x]->setTexture(tile->getNumber() + ".png");
    MutableUiBase::setTile(x, y, tile);
}


void GMXLayer2::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    _isKeyDown[static_cast<int>(keyCode)] = true;
}


void GMXLayer2::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
   _isKeyDown[static_cast<int>(keyCode)] = false;
}


void GMXLayer2::update(float dt)
{
    if ( _isKeyDown[static_cast<int>(EventKeyboard::KeyCode::KEY_RIGHT_ARROW)] ) _cameraDirection.x = 1.0f;
    else if ( _isKeyDown[static_cast<int>(EventKeyboard::KeyCode::KEY_LEFT_ARROW)] ) _cameraDirection.x = -1.0f;
    else _cameraDirection.x = 0.0f;
    
    if ( _isKeyDown[static_cast<int>(EventKeyboard::KeyCode::KEY_UP_ARROW)] ) _cameraDirection.y = 1.0f;
    else if ( _isKeyDown[static_cast<int>(EventKeyboard::KeyCode::KEY_DOWN_ARROW)] ) _cameraDirection.y = -1.0f;
    else _cameraDirection.y = 0.0f;
    
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
                fileName = _tiles[y][x]->getNumber() + ".png";
                pos = _tiles[y][x]->getPosition() - _tileRootWorldPosition;
                worldPos = _tiles[y][x]->getPosition();
            }
            
            _tileImages[i][j]->setTexture(fileName);
            _tileImages[i][j]->setPosition(pos);
            _tileIndices[i][j]->setPosition(pos);
            
            if ( viewable )
            {
//                _worldDebugNode->drawLine(Vec2(worldPos.x - _file.tileWidth / 2, worldPos.y), Vec2(worldPos.x, worldPos.y + _file.tileHeight / 2), Color4F(1, 1, 1, 0.3f));
//                _worldDebugNode->drawLine(Vec2(worldPos.x, worldPos.y + _file.tileHeight / 2), Vec2(worldPos.x + _file.tileWidth / 2, worldPos.y), Color4F(1, 1, 1, 0.3f));
//                _worldDebugNode->drawLine(Vec2(worldPos.x + _file.tileWidth / 2, worldPos.y), Vec2(worldPos.x, worldPos.y - _file.tileHeight / 2), Color4F(1, 1, 1, 0.3f));
//                _worldDebugNode->drawLine(Vec2(worldPos.x, worldPos.y - _file.tileHeight / 2), Vec2(worldPos.x - _file.tileWidth / 2, worldPos.y), Color4F(1, 1, 1, 0.3f));
//                _tileIndices[i][j]->setString("(" + std::to_string(y) + ", " + std::to_string(x) + ")");
            }
            else
            {
                _tileIndices[i][j]->setString("");
            }
        }
    }
    
    for(int i = 0 ; i < _cellSpacePartition->getNumOfCellY() * _cellSpacePartition->getNumOfCellX() ; ++ i)
    {
        Rect cellRect = _cellSpacePartition->getCell(i).boundingBox;
        _worldDebugNode->drawRect(cellRect.origin, cellRect.origin + cellRect.size, Color4F::RED);
    }
    
}


void GMXLayer2::setCenterViewParameter(const cocos2d::Vec2& p)
{
    _centerViewParam = p;
}









