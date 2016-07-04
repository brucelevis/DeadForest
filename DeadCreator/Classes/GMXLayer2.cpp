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
#include "TileToolCommand.hpp"
#include "Sheriff.hpp"
#include "Item556mm.hpp"
using namespace cocos2d;

GMXLayer2::GMXLayer2(EditScene2& imguiLayer, GMXFile& file) :
_imguiLayer(imguiLayer),
_file(file),
_worldDebugNode(nullptr),
_localDebugNode(nullptr),
_hoveredTileRegion(nullptr),
_canvasSize(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y),
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
    
    _cellSpacePartition = CellSpacePartition::create(_file.worldSize, Size(_file.tileWidth * 5, _file.tileHeight * 5));
    addChild(_cellSpacePartition);
    
    _camera = Camera2D::create();
    _camera->setPosition(_layerSize / 2);
    addChild(_camera);
    
    initFile();
    
    _paletteLayer = PaletteLayer::create();
    addChild(_paletteLayer);
    
    _navigatorLayer = NavigatorLayer::create(*this);
    addChild(_navigatorLayer);
    
    _historyLayer = HistoryLayer::create(*this);
    addChild(_historyLayer);
    
    _tileToolCommand = new TileToolCommand(this);
    
    
    auto ent = Sheriff::create(*this, 0, ui::Widget::TextureResType::PLIST);
    ent->setWorldPosition(Vec2(_layerSize / 2));
    _clipNode->addChild(ent, 100);
    
    auto item = Item556mm::create(*this, 1, "5_56mm.png", ui::Widget::TextureResType::PLIST);
    item->setWorldPosition(Vec2(_layerSize / 2) + Vec2(50, 100));
    _clipNode->addChild(item, 100);
    
    auto item2 = Item556mm::create(*this, 2, "9mm.png", ui::Widget::TextureResType::PLIST);
    item2->setWorldPosition(Vec2(_layerSize / 2) + Vec2(100, 100));
    _clipNode->addChild(item2, 100);
    
    auto item3 = Item556mm::create(*this, 3, "Shell.png", ui::Widget::TextureResType::PLIST);
    item3->setWorldPosition(Vec2(_layerSize / 2) + Vec2(150, 100));
    _clipNode->addChild(item3, 100);
    
    auto item4 = Item556mm::create(*this, 4, "Axe.png", ui::Widget::TextureResType::PLIST);
    item4->setWorldPosition(Vec2(_layerSize / 2) + Vec2(200, 100));
    _clipNode->addChild(item4, 100);
    
    auto item5 = Item556mm::create(*this, 5, "Glock17.png", ui::Widget::TextureResType::PLIST);
    item5->setWorldPosition(Vec2(_layerSize / 2) + Vec2(250, 100));
    _clipNode->addChild(item5, 100);
    
    auto item6 = Item556mm::create(*this, 6, "M16A2.png", ui::Widget::TextureResType::PLIST);
    item6->setWorldPosition(Vec2(_layerSize / 2) + Vec2(300, 100));
    _clipNode->addChild(item6, 100);
    
    auto item7 = Item556mm::create(*this, 7, "M1897.png", ui::Widget::TextureResType::PLIST);
    item7->setWorldPosition(Vec2(_layerSize / 2) + Vec2(350, 100));
    _clipNode->addChild(item7, 100);
    
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
    
    if ( _layerSize.width + _layerPosition.x > g.IO.DisplaySize.x - WINDOW_PADDING )
        _layerSize.width = g.IO.DisplaySize.x - WINDOW_PADDING - _layerPosition.x;
    
    if ( _layerSize.height + _layerPosition.y > g.IO.DisplaySize.y - WINDOW_PADDING - height )
        _layerSize.height = g.IO.DisplaySize.y - WINDOW_PADDING - _layerPosition.y - height;

    
    if ( _layerPosition.x < WINDOW_PADDING )
        _layerPosition.x = WINDOW_PADDING;
    
    if ( _layerPosition.y < height + WINDOW_PADDING + ICONBAR_HEIGHT )
        _layerPosition.y = height + WINDOW_PADDING + ICONBAR_HEIGHT;
    
    if ( _layerPosition.x + _layerSize.width > g.IO.DisplaySize.x - WINDOW_PADDING )
        _layerPosition.x = g.IO.DisplaySize.x - _layerSize.width - WINDOW_PADDING;
    
    if ( _layerPosition.y + _layerSize.height > g.IO.DisplaySize.y - WINDOW_PADDING - STATUSBAR_HEIGHT )
        _layerPosition.y = g.IO.DisplaySize.y - _layerSize.height - WINDOW_PADDING - STATUSBAR_HEIGHT;
    
    
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

    _canvasSize = Size(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
    ImGui::InvisibleButton("##dummy", ImVec2(_canvasSize.width, _canvasSize.height));
    
    _layerPosition.setPoint(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
    _layerSize.setSize(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

    _mousePosInCanvas = Vec2(ImGui::GetIO().MousePos.x - ImGui::GetCursorScreenPos().x,
                            ImGui::GetContentRegionAvail().y - (ImGui::GetIO().MousePos.y - ImGui::GetCursorScreenPos().y));
    
    _mousePosInWorld = _camera->getPosition() + (Vec2(_mousePosInCanvas.x - _layerSize.width / 2 + ImGui::GetStyle().WindowPadding.x,
                                                      _mousePosInCanvas.y - _layerSize.height / 2 + ImGui::GetStyle().WindowPadding.y));
    
    _mousePosInWorld.clamp(_camera->getPosition() - Vec2(_layerSize / 2), _camera->getPosition() + Vec2(_layerSize / 2));
 
    static Vec2 mousePosInCocos2dMatrix;
    mousePosInCocos2dMatrix = Vec2(ImGui::GetIO().MousePos.x, ImGui::GetIO().DisplaySize.y - ImGui::GetIO().MousePos.y);
    
    static bool titleClicked = false;
    if ( ImGui::GetIO().MouseClicked[0] )
    {
        Rect boundingBox(_layerPosition.x, ImGui::GetIO().DisplaySize.y - _layerPosition.y - height, _layerSize.width, height);
        if ( boundingBox.containsPoint(mousePosInCocos2dMatrix) )
        {
            titleClicked = true;
        }
        
        if ( !_isLeftMouseClickEventDone )
        {
            _tileToolCommand->begin();
            _isLeftMouseClickEventDone = true;
        }
        
    }
    else if ( ImGui::GetIO().MouseReleased[0] )
    {
        titleClicked = false;
        
        if ( _isLeftMouseClickEventDone )
        {
            _tileToolCommand->end();
            _isLeftMouseClickEventDone = false;
            
            if ( !_tileToolCommand->empty() )
            {
                _historyLayer->pushCommand(_tileToolCommand->clone());
            }
        }
    }
    
    _hoveredTileRegion->clear();
    if ( _paletteLayer->getPaletteType() == PaletteType::TILE )
    {
        auto indices = getFocusedTileIndex(_mousePosInWorld, _file.tileWidth, _file.tileHeight, DUMMY_TILE_SIZE);
        
        Vec2 hoveredRegionCenterPos = _tiles[indices.second][indices.first].getPosition();
        _hoveredTileRegion->drawSegment(hoveredRegionCenterPos + Vec2(-_file.tileWidth / 2, 0), hoveredRegionCenterPos + Vec2(0, _file.tileHeight / 2), 2.0f, Color4F::GREEN);
        _hoveredTileRegion->drawSegment(hoveredRegionCenterPos + Vec2(0, _file.tileHeight / 2), hoveredRegionCenterPos + Vec2(_file.tileWidth / 2, 0), 2.0f, Color4F::GREEN);
        _hoveredTileRegion->drawSegment(hoveredRegionCenterPos + Vec2(_file.tileWidth / 2, 0), hoveredRegionCenterPos + Vec2(0, -_file.tileHeight / 2), 2.0f, Color4F::GREEN);
        _hoveredTileRegion->drawSegment(hoveredRegionCenterPos + Vec2(0, -_file.tileHeight / 2), hoveredRegionCenterPos + Vec2(-_file.tileWidth / 2, 0), 2.0f, Color4F::GREEN);
        
        if ( (ImGui::IsMouseDragging() || ImGui::GetIO().MouseClicked[0]) && ImGui::IsMouseHoveringWindow() && !titleClicked )
        {
            Vec2 resizeButtonOrigin(_layerPosition.x + _layerSize.width - 30, ImGui::GetIO().DisplaySize.y - _layerPosition.y - _layerSize.height);
            bool isClickedResizeButton = Rect(resizeButtonOrigin.x, resizeButtonOrigin.y, 30, 30).containsPoint(mousePosInCocos2dMatrix);
            if ( !isClickedResizeButton )
            {
                TileType selectedTile = static_cast<TileType>(_paletteLayer->getSelectedItem());
                putTile(selectedTile, indices.first, indices.second);
            }
        }
    }

    
    ImGui::End();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
    
    setPosition(_layerPosition.x, Director::getInstance()->getVisibleSize().height - _layerPosition.y - _layerSize.height);
    _tileRoot->setPosition(_layerSize / 2);
    
    _clipNode->setClippingRegion(Rect(0, 0, _layerSize.width, _layerSize.height));
    
    _localDebugNode->clear();
    _localDebugNode->drawDot(Vec2(_layerSize / 2), 5.0f, Color4F::YELLOW);
    
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
    if ( _isShowHistory ) _historyLayer->showLayer(&_isShowHistory);
}



void GMXLayer2::setTile(int x, int y, const TileBase& tile, bool isExecCommand)
{
    if ( !isExecCommand )
    {
        _tileToolCommand->pushTile(_tiles[y][x], tile);
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
                fileName = _tiles[y][x].getNumber() + ".png";
                pos = _tiles[y][x].getPosition() - _tileRootWorldPosition;
                worldPos = _tiles[y][x].getPosition();
            }
            
            _tileImages[i][j]->setTexture(fileName);
            _tileImages[i][j]->setPosition(pos);
            _tileIndices[i][j]->setPosition(pos);
            
            if ( viewable )
            {
                _worldDebugNode->drawLine(Vec2(worldPos.x - _file.tileWidth / 2, worldPos.y), Vec2(worldPos.x, worldPos.y + _file.tileHeight / 2), Color4F(1, 1, 1, 0.3f));
                _worldDebugNode->drawLine(Vec2(worldPos.x, worldPos.y + _file.tileHeight / 2), Vec2(worldPos.x + _file.tileWidth / 2, worldPos.y), Color4F(1, 1, 1, 0.3f));
                _worldDebugNode->drawLine(Vec2(worldPos.x + _file.tileWidth / 2, worldPos.y), Vec2(worldPos.x, worldPos.y - _file.tileHeight / 2), Color4F(1, 1, 1, 0.3f));
                _worldDebugNode->drawLine(Vec2(worldPos.x, worldPos.y - _file.tileHeight / 2), Vec2(worldPos.x - _file.tileWidth / 2, worldPos.y), Color4F(1, 1, 1, 0.3f));
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
        _worldDebugNode->drawRect(cellRect.origin, cellRect.origin + cellRect.size, Color4F(1, 0, 0, 0.3f));
    }
    
}


void GMXLayer2::setCenterViewParameter(const cocos2d::Vec2& p)
{
    _centerViewParam = p;
}


void GMXLayer2::putTile(TileType type, int x, int y)
{
    if ( _tiles[y][x].getType() == type && _tiles[y][x].getTileTail() == "1234" )
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
                        s.push(TileBase(nei[0].first, nei[0].second, TileBase::getTileHeader(TileType::DIRT) + "2", indexToPosition(nei[0].first, nei[0].second, tileWidth, tileHeight, dummy)));
                        s.push(TileBase(nei[1].first, nei[1].second, TileBase::getTileHeader(TileType::DIRT) + "23", indexToPosition(nei[1].first, nei[1].second, tileWidth, tileHeight, dummy)));
                        s.push(TileBase(nei[7].first, nei[7].second, TileBase::getTileHeader(TileType::DIRT) + "12", indexToPosition(nei[7].first, nei[7].second, tileWidth, tileHeight, dummy)));
                    }
                    else if ( tail == "23")
                    {
                        s.push(TileBase(nei[1].first, nei[1].second, TileBase::getTileHeader(TileType::DIRT) + "23", indexToPosition(nei[1].first, nei[1].second, tileWidth, tileHeight, dummy)));
                    }
                    else if ( tail == "3")
                    {
                        s.push(TileBase(nei[1].first, nei[1].second, TileBase::getTileHeader(TileType::DIRT) + "23", indexToPosition(nei[1].first, nei[1].second, tileWidth, tileHeight, dummy)));
                        s.push(TileBase(nei[2].first, nei[2].second, TileBase::getTileHeader(TileType::DIRT) + "3", indexToPosition(nei[2].first, nei[2].second, tileWidth, tileHeight, dummy)));
                        s.push(TileBase(nei[3].first, nei[3].second, TileBase::getTileHeader(TileType::DIRT) + "34", indexToPosition(nei[3].first, nei[3].second, tileWidth, tileHeight, dummy)));
                    }
                    else if ( tail == "34")
                    {
                        s.push(TileBase(nei[3].first, nei[3].second, TileBase::getTileHeader(TileType::DIRT) + "34", indexToPosition(nei[3].first, nei[3].second, tileWidth, tileHeight, dummy)));
                    }
                    else if ( tail == "4")
                    {
                        s.push(TileBase(nei[3].first, nei[3].second, TileBase::getTileHeader(TileType::DIRT) + "34", indexToPosition(nei[3].first, nei[3].second, tileWidth, tileHeight, dummy)));
                        s.push(TileBase(nei[4].first, nei[4].second, TileBase::getTileHeader(TileType::DIRT) + "4", indexToPosition(nei[4].first, nei[4].second, tileWidth, tileHeight, dummy)));
                        s.push(TileBase(nei[5].first, nei[5].second, TileBase::getTileHeader(TileType::DIRT) + "14", indexToPosition(nei[5].first, nei[5].second, tileWidth, tileHeight, dummy)));
                    }
                    else if ( tail == "14")
                    {
                        s.push(TileBase(nei[5].first, nei[5].second, TileBase::getTileHeader(TileType::DIRT) + "14", indexToPosition(nei[5].first, nei[5].second, tileWidth, tileHeight, dummy)));
                    }
                    else if ( tail == "1")
                    {
                        s.push(TileBase(nei[5].first, nei[5].second, TileBase::getTileHeader(TileType::DIRT) + "14", indexToPosition(nei[5].first, nei[5].second, tileWidth, tileHeight, dummy)));
                        s.push(TileBase(nei[6].first, nei[6].second, TileBase::getTileHeader(TileType::DIRT) + "1", indexToPosition(nei[6].first, nei[6].second, tileWidth, tileHeight, dummy)));
                        s.push(TileBase(nei[7].first, nei[7].second, TileBase::getTileHeader(TileType::DIRT) + "12", indexToPosition(nei[7].first, nei[7].second, tileWidth, tileHeight, dummy)));
                    }
                    else if ( tail == "12")
                    {
                        s.push(TileBase(nei[7].first, nei[7].second, TileBase::getTileHeader(TileType::DIRT) + "12", indexToPosition(nei[7].first, nei[7].second, tileWidth, tileHeight, dummy)));
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
                if ( originalTail.empty() ) finalTile = TileBase::getTileHeader(TileType::DIRT) + "1234";
                else finalTile = _tiles[yy][xx].getTileHeader() + originalTail;
                
                temp.setNumber(finalTile);
                setTile(xx, yy, temp);
            }
        }
    }
}








