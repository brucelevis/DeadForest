//
//  GMXLayer2.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 28..
//
//

#include "GMXLayer2.hpp"
#include "TerrainBase.hpp"
#include "GMXFile.hpp"
using namespace cocos2d;


GMXLayer2::GMXLayer2(GMXFile& file) :
_file(file),
_debugNode(nullptr),
_visibleSize(Director::getInstance()->getVisibleSize()),
_layerSize(Size(800, 500)),
_layerMaxSize(Director::getInstance()->getVisibleSize()),
_layerPosition(Vec2::ZERO),
_centerViewParam(Vec2::ZERO),
_centerViewPosition(Vec2(_layerSize / 2)),
_cameraDirection(Vec2::ZERO),
_camera(nullptr),
_windowSpeed(1000),
_root(nullptr)
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
    
    _root = Node::create();
    _root->setPosition(_layerSize / 2);
    _clipNode->addChild(_root);
    
    _debugNode = DrawNode::create();
    addChild(_debugNode);
    
    _cellSpacePartition = CellSpacePartition::create(_file.worldSize, Size(1000, 1000));
    addChild(_cellSpacePartition);
    
    _camera = Camera2D::create();
    _camera->setPosition(_centerViewPosition);
    addChild(_camera);
    
    initFile();
    
    return true;
}


void GMXLayer2::initFile()
{
    int x = _file.numOfTileX;
    int y = _file.numOfTileY;
    
    int viewX = x;
    int viewY = y;
    
    _tileImages.resize(viewY);
    for(int i = 0 ; i < viewY; ++ i)
    {
        _tileImages[i].resize(viewX);
    }
    
    for(int i = 0 ; i < viewY; ++ i)
    {
        for(int j = 0 ; j < viewX; ++ j)
        {
            Vec2 tilePosition;
            if ( i % 2 == 0)
            {
                tilePosition.setPoint(j * _file.tileWidth, i * _file.tileHeight / 2);
            }
            else
            {
                tilePosition.setPoint(j * _file.tileWidth + _file.tileWidth / 2, i * _file.tileHeight / 2);
            }
            
            _tileImages[i][j] = TileImage::create(*this, "1_1_1234.png");
            _tileImages[i][j]->setWorldPosition(tilePosition);
            _root->addChild(_tileImages[i][j]);
        }
    }
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
    _root->setPosition(_layerSize / 2);
    
    _clipNode->setClippingRegion(Rect(0, 0, _layerSize.width, _layerSize.height));
    
        _debugNode->clear();
        _debugNode->drawDot(Vec2::ZERO, 4.0f, Color4F::YELLOW);
        _debugNode->drawRect(Vec2::ZERO, _layerSize, Color4F::RED);
    //
    //    log("layer: %.0f, %.0f", _layerPosition.x, _layerPosition.y);
    //    log("pos: %.0f, %.0f", getPosition().x, getPosition().y);
    
    
    if ( _isShowWindow == false )
        setVisible(false);
}


void GMXLayer2::setVisible(bool visible)
{
    _isShowWindow = visible;
    Node::setVisible(visible);
}


void GMXLayer2::setTerrain(int x, int y, TerrainBase* terrain)
{
    _tileImages[y][x]->setTexture(terrain->getNumber() + ".png");
    MutableUiBase::setTerrain(x, y, terrain);
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
    
    _centerViewParam = Vec2( _camera->getPosition().x / (_file.worldSize.width - _layerSize.width),
                            _camera->getPosition().y / (_file.worldSize.height - _layerSize.height));

    Vec2 oldPosition = _camera->getPosition();
    Vec2 newPosition = _camera->getPosition() + (_cameraDirection * _windowSpeed * dt);

    if ( _cellSpacePartition->isUpdateChunk(oldPosition, newPosition) )
    {
        _rootTileWorldPosition = _camera->getPosition();
    }
    
    _root->setPosition( Vec2(_layerSize / 2) + _rootTileWorldPosition - _camera->getPosition() );
}






