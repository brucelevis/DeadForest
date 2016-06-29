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
using namespace cocos2d;

GMXLayer2::GMXLayer2(EditScene2& imguiLayer, GMXFile& file) :
_imguiLayer(imguiLayer),
_file(file),
_worldDebugNode(nullptr),
_localDebugNode(nullptr),
_visibleSize(Director::getInstance()->getVisibleSize()),
_layerSize(Size(800, 500)),
_layerPosition(Director::getInstance()->getVisibleSize() / 2 - _layerSize / 2),
_centerViewParam(Vec2::ZERO),
_centerViewPosition(Vec2(_layerSize / 2)),
_cameraDirection(Vec2::ZERO),
_camera(nullptr),
_windowSpeed(1000),
_tileRoot(nullptr),
_viewX(30),
_viewY(60)
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
    
    _cellSpacePartition = CellSpacePartition::create(_file.worldSize, Size(500, 500));
    addChild(_cellSpacePartition);
    
    _camera = Camera2D::create();
    _camera->setPosition(_centerViewPosition);
    addChild(_camera);
    
    initFile();
    
    _paletteLayer = PaletteLayer::create(_imguiLayer);
    addChild(_paletteLayer);
    
    _navigatorLayer = NavigatorLayer::create(_imguiLayer);
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
    if ( _isShowPalette ) _paletteLayer->showLayer(&_isShowPalette);
    if ( _isShowNavigator ) _navigatorLayer->showLayer(&_isShowNavigator);
    
    ImGui::SetNextWindowPos(ImVec2(_layerPosition.x, _layerPosition.y), ImGuiSetCond_Appearing);
    ImGui::SetNextWindowSize(ImVec2(_layerSize.width, _layerSize.height), ImGuiSetCond_Appearing);
    
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.8200000, 0.8200000, 0.8200000, 1.0000000));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(200,200));
    ImGui::Begin(_file.fileName.c_str(), &_isShowWindow, ImVec2(0,0), 0.0f,
                 ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoBringToFrontOnFocus);
    
    _layerPosition.setPoint(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
    _layerSize.setSize(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
    
    ImGui::End();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    setPosition(_layerPosition.x, _visibleSize.height - _layerPosition.y - _layerSize.height);
    _tileRoot->setPosition(_layerSize / 2);
    
    _clipNode->setClippingRegion(Rect(0, 0, _layerSize.width, _layerSize.height));
    
    _worldDebugNode->clear();
    for(int i = 0 ; i < _cellSpacePartition->getNumOfCellY() * _cellSpacePartition->getNumOfCellX() ; ++ i)
    {
        Rect cellRect = _cellSpacePartition->getCell(i).boundingBox;
        _worldDebugNode->drawRect(cellRect.origin, cellRect.origin + cellRect.size, Color4F::RED);
    }
    
    _localDebugNode->clear();
    _localDebugNode->drawDot(Vec2(_layerSize / 2), 5.0f, Color4F::YELLOW);
    
//        log("layer: %.0f, %.0f", _layerPosition.x, _layerPosition.y);
//        log("pos: %.0f, %.0f", getPosition().x, getPosition().y);
    
    if ( _isShowWindow == false )
    {
        setVisible(false);
        _isShowPalette = false;
        _isShowNavigator = false;
        _imguiLayer.setEnableEditMenu(false);
        _imguiLayer.setEnablePlayerMenu(false);
        _imguiLayer.setEnableWindowMenu(false);
    }
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
    
    Vec2 oldPosition = _camera->getPosition();
    Vec2 newPosition = _camera->getPosition() + (_cameraDirection * _windowSpeed * dt);
    
    _centerViewParam = Vec2( newPosition.x / (_file.worldSize.width - _layerSize.width),
                            newPosition.y / (_file.worldSize.height - _layerSize.height));
    
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
    
    for(int i = 0 ; i < _viewY; ++ i)
    {
        for(int j = 0 ; j < _viewX; ++ j)
        {
            int x = index.first + j;
            int y = index.second + i;
            
            std::string fileName;
            Vec2 pos;
            if ( x < 0 || x > _file.numOfTileX + DUMMY_TILE_SIZE * 2 - 1 ||
                 y < 0 || y > _file.numOfTileY * 2 + DUMMY_TILE_SIZE * 4- 1)
            {
                fileName = "empty_image.png";
                pos = Vec2::ZERO;
            }
            else
            {
                fileName = _tiles[y][x]->getNumber() + ".png";
                pos = _tiles[y][x]->getPosition() - _tileRootWorldPosition;
            }
            
            _tileImages[i][j]->setTexture(fileName);
            _tileImages[i][j]->setPosition(pos);
        }
    }
    
}












