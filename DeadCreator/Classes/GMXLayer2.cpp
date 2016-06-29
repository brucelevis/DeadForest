//
//  GMXLayer2.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 28..
//
//

#include "GMXLayer2.hpp"
#include "TileBase.hpp"
#include "GMXFile.hpp"
using namespace cocos2d;


GMXLayer2::GMXLayer2(GMXFile& file) :
_file(file),
_worldDebugNode(nullptr),
_localDebugNode(nullptr),
_visibleSize(Director::getInstance()->getVisibleSize()),
_layerSize(Size(800, 500)),
_layerMaxSize(Director::getInstance()->getVisibleSize()),
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


GMXLayer2* GMXLayer2::create(GMXFile& file)
{
    auto ret = new (std::nothrow) GMXLayer2(file);
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
    if ( !ImGuiLayer::init() )
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
    
    addImGUI([this] {
        
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
            
            _tiles[i][j] = new TileBase( std::to_string(random(1,3)) + "_" + std::to_string(random(1,3)) + "_1234", tilePosition);
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
    ImGui::SetNextWindowPos(ImVec2(_layerPosition.x, _layerPosition.y), ImGuiSetCond_Appearing);
    ImGui::SetNextWindowSize(ImVec2(_layerSize.width, _layerSize.height), ImGuiSetCond_Appearing);
    
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.8200000, 0.8200000, 0.8200000, 1.0000000));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::Begin("gmx layer", &_isShowWindow, ImVec2(0,0), 0.0f,
                 ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_ShowBorders |
                 ImGuiWindowFlags_NoBringToFrontOnFocus);
    
    _layerPosition.setPoint(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
    _layerSize.setSize(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
    
    ImGui::End();
    ImGui::PopStyleVar();
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
        setVisible(false);
}


void GMXLayer2::setVisible(bool visible)
{
    _isShowWindow = visible;
    Node::setVisible(visible);
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
    cameraPos.clamp(Vec2::ZERO, Vec2(_file.worldSize));
    _camera->setPosition(cameraPos);
    
    if ( _cellSpacePartition->isUpdateChunk(oldPosition, newPosition) )
    {
        updateChunk(newPosition);
    }
    
    _tileRoot->setPosition( Vec2(_layerSize / 2) + _tileRootWorldPosition - _camera->getPosition() );
    
    _rootNode->setPosition( -_camera->getPosition() + Vec2(_layerSize / 2) );
    log("%.0f, %.0f", _camera->getPosition().x, _camera->getPosition().y);
    
}


void GMXLayer2::updateChunk(const cocos2d::Vec2& pivot)
{
    _tileRootWorldPosition = pivot;
    
    auto index = getFocusedTileIndex(pivot);
    
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


std::pair<int,int> GMXLayer2::getFocusedTileIndex(const cocos2d::Vec2& worldPos) const
{
    int centerTileIndexX = static_cast<int>((worldPos.x) / _file.tileWidth) + DUMMY_TILE_SIZE;        // not exact index!
    int centerTileIndexY = static_cast<int>((worldPos.y) / (_file.tileHeight / 2))  + DUMMY_TILE_SIZE * 2;  // not exact index!
    
    for(int i = centerTileIndexY - 2 ; i < centerTileIndexY + 2 ; ++ i)
    {
        for(int j = centerTileIndexX - 2 ; j < centerTileIndexX + 2 ; ++ j)
        {
            Vec2 pivot;
            if( i % 2 == 0 ) pivot.setPoint(j * 128 - (128 * DUMMY_TILE_SIZE), i * 64 - (128 * DUMMY_TILE_SIZE));
            else pivot.setPoint(64 + j * 128 - (128 * DUMMY_TILE_SIZE), i * 64 - (128 * DUMMY_TILE_SIZE));
            
            if(isContainPointInDiamond(pivot, Size(_file.tileWidth / 2, _file.tileHeight / 2), worldPos))
            {
                centerTileIndexX = j; // exact index!
                centerTileIndexY = i; // exact index!
                return std::make_pair(j, i);
            }
        }
    }
    
    return {0, 0};
}


bool GMXLayer2::isContainPointInDiamond(const cocos2d::Vec2& diamondCenter, const cocos2d::Size& halfLen, const cocos2d::Vec2& p) const
{
    int m = 1;
    float b[4];
    
    cocos2d::Vec2 leftPoint = cocos2d::Vec2(diamondCenter.x - halfLen.width, diamondCenter.y);
    cocos2d::Vec2 rightPoint = cocos2d::Vec2(diamondCenter.x + halfLen.width, diamondCenter.y);
    cocos2d::Vec2 topPoint = cocos2d::Vec2(diamondCenter.x, diamondCenter.y + halfLen.height);
    cocos2d::Vec2 botPoint = cocos2d::Vec2(diamondCenter.x, diamondCenter.y - halfLen.height);
    
    b[0] = leftPoint.y - m * leftPoint.x;
    b[1] = topPoint.y + m * topPoint.x;
    b[2] = rightPoint.y - m * rightPoint.x;
    b[3] = botPoint.y + m * botPoint.x;
    
    if (p.y - (m * p.x) - b[0] < 0.f
        && p.y - (-m * p.x) - b[1] < 0.f
        && p.y - (m * p.x) - b[2] > 0.f
        && p.y - (-m * p.x) - b[3] > 0.f)
    {
        return true;
    }
    
    return false;
}




