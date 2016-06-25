#include <fstream>
using namespace std;

#include "FileSystem.hpp"

#include "HelloWorldScene.h"
#include "GMXLayer.hpp"
#include "CCImGui.h"
#include "SizeProtocol.h"
#include "PaletteWindow.hpp"
#include "TriggerEditor.hpp"
#include "OpenFileWindow.hpp"
#include "ImGuiGLViewImpl.h"
using namespace cocos2d;


HelloWorld::HelloWorld()
{
}


cocos2d::Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}


bool HelloWorld::init()
{
    if ( !ImGuiLayer::init() )
    {
        return false;
    }
    
    for(int i = 0 ; i < 256 ; ++ i) _isKeyPressed[i] = false;
    _isMousePressed = false;
    
    auto keylistener = EventListenerKeyboard::create();
    keylistener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
    keylistener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);
    
    auto mouselistener = EventListenerMouse::create();
    mouselistener->onMouseDown = CC_CALLBACK_1(HelloWorld::onMouseDown, this);
    mouselistener->onMouseUp = CC_CALLBACK_1(HelloWorld::onMouseUp, this);
    mouselistener->onMouseMove = CC_CALLBACK_1(HelloWorld::onMouseMove, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouselistener, this);
    
    _oldWindowSize = Director::getInstance()->getVisibleSize();
    
    CCIMGUI->addImGUI([this](){
        
        // main menu
        if ( _showNewMap ) showNewMapWindow(&_showNewMap);
        if ( _showPalette ) _palette->showPaletteWindow(&_showPalette);
        if ( _showFileMenuBar ) showFileMenuBar(&_showFileMenuBar);
        if ( _showTrigger ) _triggerEditor->showTriggerEditor(&_showTrigger);
        if ( _showOpenMap ) _openFileWindow->showOpenFileWindow(&_showOpenMap);
        
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File", _isFileEnable))
            {
                if (ImGui::MenuItem("New", "Ctrl+N", &_showNewMap))
                {
                    if ( _showNewMap ) _isFileEnable = false;
                }
                if (ImGui::MenuItem("Open", "Ctrl+O", &_showOpenMap))
                {
                    if ( _showOpenMap ) _isFileEnable = false;
                }
                if (ImGui::BeginMenu("Open Recent"))
                {
                    ImGui::MenuItem("fish_hat.c");
                    ImGui::MenuItem("fish_hat.inl");
                    ImGui::MenuItem("fish_hat.h");
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Save", "Ctrl+S", false, _isUndo)) { save(); }
                if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S", false, _isUndo)) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Quit", "Alt+F4")) {}
                
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit", _isEditEnable))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z", false, _isUndo)) { undo(); }
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, _isRedo)) { redo(); }
                
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X", false, false)) {}
                if (ImGui::MenuItem("Copy", "CTRL+C", false, false)) {}
                if (ImGui::MenuItem("Paste", "CTRL+V", false, false)) {}
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Players", _isPlayerEnable))
            {
                if (ImGui::MenuItem("Player 1")) {}
                if (ImGui::MenuItem("Player 2")) {}
                if (ImGui::MenuItem("Player 3")) {}
                if (ImGui::MenuItem("Player 4")) {}
                if (ImGui::MenuItem("Player 5")) {}
                if (ImGui::MenuItem("Player 6")) {}
                if (ImGui::MenuItem("Player 7")) {}
                if (ImGui::MenuItem("Player 8")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Setting")) {}
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Windows", _isWindowEnable))
            {
                ImGui::MenuItem("Trigger Editor", "SHIFT+T", &_showTrigger);
                ImGui::MenuItem("Palette Window", "SHIFT+P", &_showPalette);
                ImGui::MenuItem("Property Editor", "SHIFT+R", &_showProperty);
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
        
        // bottom menu
        ImGuiIO& io = ImGui::GetIO();
        
        ImGui::SetNextWindowPos(ImVec2(0.0f, io.DisplaySize.y - STATUSBAR_HEIGHT));
        ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, STATUSBAR_HEIGHT));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::Begin("##BottomMenuBar", NULL,
                     ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoSavedSettings);
        
        ImGui::Columns(5, "extra_info", false);
        ImGui::SameLine();
        ImGui::Text("Mouse Position (%.0f, %.0f)",
                    cocos2d::clampf(ImGui::GetMousePos().x, 0.0f, io.DisplaySize.x),
                    cocos2d::clampf(io.DisplaySize.y - ImGui::GetMousePos().y, 0.0f, io.DisplaySize.y));
        
        ImGui::SameLine();
        ImGui::NextColumn();
        ImGui::Text("World Position (%.0f, %.0f)", _worldPosition.x, _worldPosition.y);
        
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
        
        // property ui
        ImGuiStyle& style = ImGui::GetStyle();
        
        ImGui::SetNextWindowPos(ImVec2(WINDOW_PADDING, MENUBAR_HEIGHT + MINIMAP_SIZE + WINDOW_PADDING * 2));
        ImGui::SetNextWindowSize(ImVec2(MINIMAP_SIZE, io.DisplaySize.y - (MENUBAR_HEIGHT + MINIMAP_SIZE + STATUSBAR_HEIGHT + WINDOW_PADDING * 3)));
        ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.8200000, 0.8200000, 0.8200000, 1.0000000));
        if (ImGui::Begin(" Entities", NULL,
                         ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoBringToFrontOnFocus |
                         ImGuiWindowFlags_NoMove))
        {
            static int selected = 0;
            ImGui::BeginChild("##ObjectList", ImVec2(MINIMAP_SIZE - style.WindowPadding.x * 2, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
            for (int i = 0; i < 1; i++)
            {
                std::string label;
                label = "My Object " + std::to_string(i);
                if (ImGui::Selectable(label.c_str(), selected == i))
                    selected = i;
            }
            ImGui::EndChild();
        }
        ImGui::End();
        ImGui::PopStyleColor(1);
        
    }, "ui_init");
    
    
    CCIMGUI->addImGUI([this]{
        
        static bool isShowDemo = true;
        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
        ImGui::ShowTestWindow(&isShowDemo);
        
    }, "test_window");
    
    _gmxLayerManager = GMXLayerManager::create();
    _gmxLayerManager->setPosition(Vec2(MINIMAP_SIZE + WINDOW_PADDING * 2, STATUSBAR_HEIGHT + WINDOW_PADDING));
    addChild(_gmxLayerManager);
    
    _minimapLayer = MinimapLayer::create(Size(MINIMAP_SIZE, MINIMAP_SIZE));
    _minimapLayer->setPosition(Vec2(WINDOW_PADDING, _director->getVisibleSize().height - MENUBAR_HEIGHT - MINIMAP_SIZE - WINDOW_PADDING));
    addChild(_minimapLayer);

    
    _debugNode = DrawNode::create();
    _debugNode->setPosition(Vec2(MINIMAP_SIZE + WINDOW_PADDING * 2, STATUSBAR_HEIGHT + WINDOW_PADDING));
    addChild(_debugNode);
    
    _openFileWindow = OpenFileWindow::create();
    addChild(_openFileWindow);
    
    this->scheduleUpdate();
    
    return true;
}


void HelloWorld::update(float dt)
{
//    static int t = 0;
//    if ( t == 0 )
//    {
//        auto view = static_cast<ImGuiGLViewImpl*>(Director::getInstance()->getOpenGLView());
//        auto mainWindow = view->getWindow();
//        
//        view->onGLFWframebuffersize(mainWindow, 1600, 960);
//        view->setFrameSize(1600, 960);
//        view->setDesignResolutionSize(1600, 960, ResolutionPolicy::SHOW_ALL);
//        
//        onResize();
//        
//        t = 1;
//    }
    
    static auto director = Director::getInstance();
    Size currSize = director->getVisibleSize();
    if ( _oldWindowSize.width != currSize.width || _oldWindowSize.height != currSize.height)
    {
        // resize
        onResize();
    }
    
    _oldWindowSize = director->getVisibleSize();
    
    GMXLayer* currLayer = _gmxLayerManager->getCurrentLayer();
    
    if ( _isKeyPressed[static_cast<int>(EventKeyboard::KeyCode::KEY_LEFT_ARROW)] == true )
    {
        if ( currLayer )
        {
            _viewSpaceParams.x = _viewSpaceParams.x - (3000.0f / currLayer->getWorldSize().width) * dt;
            _viewSpaceParams.x = clampf(_viewSpaceParams.x, 0.0f, 1.0f);
            
            onCenterView();
        }
    }
    else if ( _isKeyPressed[static_cast<int>(EventKeyboard::KeyCode::KEY_RIGHT_ARROW)] == true )
    {
        if ( currLayer )
        {
            _viewSpaceParams.x = _viewSpaceParams.x + (3000.0f / currLayer->getWorldSize().width) * dt;
            _viewSpaceParams.x = clampf(_viewSpaceParams.x, 0.0f, 1.0f);
            
            onCenterView();
        }
    }
    
    if ( _isKeyPressed[static_cast<int>(EventKeyboard::KeyCode::KEY_DOWN_ARROW)] == true )
    {
        if ( currLayer )
        {
            _viewSpaceParams.y = _viewSpaceParams.y - (3000.0f / currLayer->getWorldSize().height) * dt;
            _viewSpaceParams.y = clampf(_viewSpaceParams.y, 0.0f, 1.0f);
            
            onCenterView();
        }
    }
    else if ( _isKeyPressed[static_cast<int>(EventKeyboard::KeyCode::KEY_UP_ARROW)] == true )
    {
        if ( currLayer )
        {
            _viewSpaceParams.y = _viewSpaceParams.y + (3000.0f / currLayer->getWorldSize().height) * dt;
            _viewSpaceParams.y = clampf(_viewSpaceParams.y, 0.0f, 1.0f);
            
            onCenterView();
        }
    }
    
    if ( _gmxLayerManager->getCurrentLayer() )
    {
        _isUndo = _gmxLayerManager->getCurrentLayer()->isUndo();
        _isRedo = _gmxLayerManager->getCurrentLayer()->isRedo();
    }
}


void HelloWorld::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    _isKeyPressed[static_cast<int>(keyCode)] = true;
    
    if ( _isCtrl && keyCode == EventKeyboard::KeyCode::KEY_N )
    {
        _showNewMap = true;
    }
    if ( _isCtrl && keyCode == EventKeyboard::KeyCode::KEY_Z )
    {
        undo();
    }
    
    if ( _isCtrl && keyCode == EventKeyboard::KeyCode::KEY_Y )
    {
        redo();
    }
    
    if ( keyCode == EventKeyboard::KeyCode::KEY_CTRL ) { _isCtrl = true; }
    else if ( keyCode == EventKeyboard::KeyCode::KEY_SHIFT ) { _isShift = true; }
}


void HelloWorld::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    _isKeyPressed[static_cast<int>(keyCode)] = false;
    if ( keyCode == EventKeyboard::KeyCode::KEY_CTRL ) _isCtrl = false;
    else if ( keyCode == EventKeyboard::KeyCode::KEY_SHIFT ) _isShift = false;
}


void HelloWorld::onMouseDown(cocos2d::Event* event)
{
    _isMousePressed = true;
    
    auto mouseEvent = static_cast<EventMouse*>(event);
    _mousePosition.setPoint(mouseEvent->getCursorX(), mouseEvent->getCursorY());
    
    bool inImGuiWidgets = ImGui::IsPosHoveringAnyWindow(ImVec2(mouseEvent->getLocationInView().x,
                                                               _director->getVisibleSize().height - mouseEvent->getLocationInView().y));
    
    if ( inImGuiWidgets ) return ;
    
    Rect minimapRect(_minimapLayer->getPosition().x, _minimapLayer->getPosition().y, MINIMAP_SIZE, MINIMAP_SIZE);
    if ( minimapRect.containsPoint(_mousePosition) )
    {
        Size focusWindowSize = _minimapLayer->getFocusWindowSize();
        Vec2 innerPosition = _mousePosition - _minimapLayer->getPosition();
        _viewSpaceParams.setPoint((innerPosition.x - focusWindowSize.width / 2) / (MINIMAP_SIZE - focusWindowSize.width),
                                  (innerPosition.y - focusWindowSize.height / 2) / (MINIMAP_SIZE - focusWindowSize.height));
        
        _viewSpaceParams.x = clampf(_viewSpaceParams.x, 0.0, 1.0);
        _viewSpaceParams.y = clampf(_viewSpaceParams.y, 0.0, 1.0);
        
        onCenterView();
    }
    
    auto currLayer = _gmxLayerManager->getCurrentLayer();
    if ( currLayer )
    {
        if ( _palette->getPaletteType() == PaletteType::TILE )
        {
            Size clipSize = currLayer->getClippingRegion().size;
            Vec2 positionInClipRect(_mousePosition - Vec2(MINIMAP_SIZE + WINDOW_PADDING * 2, STATUSBAR_HEIGHT + WINDOW_PADDING));
            
            if ( currLayer->getClippingRegion().containsPoint(positionInClipRect) )
            {
                _worldPosition = _gmxLayerManager->getCurrentLayer()->getCenterViewPosition();
                positionInClipRect -= clipSize / 2;
                _worldPosition += positionInClipRect;
                
                auto indices = currLayer->getFocusedTileIndex(_worldPosition);
                currLayer->putTile(_palette->getSelectedItem(), indices.first, indices.second);
            }
        }
    }
}


void HelloWorld::onMouseMove(cocos2d::Event* event)
{
    auto mouseEvent = static_cast<EventMouse*>(event);
    _mousePosition.setPoint(mouseEvent->getCursorX(), mouseEvent->getCursorY());
    
    if ( _isMousePressed )
    {
        Rect minimapRect(_minimapLayer->getPosition().x, _minimapLayer->getPosition().y, MINIMAP_SIZE, MINIMAP_SIZE);
        if ( minimapRect.containsPoint(_mousePosition) )
        {
            Size focusWindowSize = _minimapLayer->getFocusWindowSize();
            Vec2 innerPosition = _mousePosition - _minimapLayer->getPosition();
            _viewSpaceParams.setPoint((innerPosition.x - focusWindowSize.width / 2) / (MINIMAP_SIZE - focusWindowSize.width),
                                      (innerPosition.y - focusWindowSize.height / 2) / (MINIMAP_SIZE - focusWindowSize.height));
            
            _viewSpaceParams.x = clampf(_viewSpaceParams.x, 0.0, 1.0);
            _viewSpaceParams.y = clampf(_viewSpaceParams.y, 0.0, 1.0);
        }
    }
    
    onCenterView();
}


void HelloWorld::onMouseUp(cocos2d::Event* event)
{
    _isMousePressed = false;
}


void HelloWorld::onCenterView()
{
    _gmxLayerManager->onCenterView(_viewSpaceParams.x, _viewSpaceParams.y);
    
    if ( _minimapLayer ) _minimapLayer->onCenterView(_viewSpaceParams);
    
    auto currLayer = _gmxLayerManager->getCurrentLayer();
    if ( currLayer )
    {
        if ( _palette->getPaletteType() == PaletteType::TILE )
        {
            Size clipSize = currLayer->getClippingRegion().size;
            Vec2 positionInClipRect(_mousePosition - Vec2(MINIMAP_SIZE + WINDOW_PADDING * 2, STATUSBAR_HEIGHT + WINDOW_PADDING));
            positionInClipRect.x = clampf(positionInClipRect.x, 0.0f, clipSize.width);
            positionInClipRect.y = clampf(positionInClipRect.y, 0.0f, clipSize.height);
            
            _worldPosition = _gmxLayerManager->getCurrentLayer()->getCenterViewPosition();
            positionInClipRect -= clipSize / 2;
            _worldPosition += positionInClipRect;
            
            auto indices = currLayer->getFocusedTileIndex(_worldPosition);
            currLayer->drawSelectRegion(indices.first, indices.second);
            currLayer->setSelectTileImage(_palette->getSelectedItem());
        }
        else if ( _palette->getPaletteType() == PaletteType::ENTITY )
        {
            currLayer->disableSelectRegion();
        }
        else if ( _palette->getPaletteType() == PaletteType::ITEM )
        {
            currLayer->disableSelectRegion();
        }
        else if ( _palette->getPaletteType() == PaletteType::DOODAD )
        {
            currLayer->disableSelectRegion();
        }
    }
}


void HelloWorld::onResize()
{
    if ( _gmxLayerManager->getCurrentLayer() )
    {
        _gmxLayerManager->onResize();
    }
    _minimapLayer->onResize();
}


void HelloWorld::showNewMapWindow(bool* opened)
{
    Vec2 windowSize = Vec2(430, 430);
    ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y));
    ImGui::SetNextWindowPos(ImVec2((_oldWindowSize.width - windowSize.x) / 2, (_oldWindowSize.height - windowSize.y) / 2));
    if (!ImGui::Begin("New Map", opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
    {
        ImGui::End();
        return;
    }
    
    ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
    ImGui::BeginChild("##TileSize", ImVec2(0,60), true);
    ImGui::Text("Tile Size");
    ImGui::Columns(2, "##col1", false);
    
    static int tileSizeXItem = 0;
    const char* items1[] = {"128"};
    ImGui::Combo("width", &tileSizeXItem, items1, 1);
    ImGui::NextColumn();
    
    static int tileSizeYItem = 0;
    const char* items2[] = {"128"};
    ImGui::Combo("height", &tileSizeYItem, items2, 1);
    
    ImGui::EndChild();
    ImGui::PopStyleVar();
    
    ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
    ImGui::BeginChild("##Number Of Tiles", ImVec2(0,60), true);
    
    ImGui::Text("Number Of Tiles");
    ImGui::Columns(2, "##col2", false);
    
    static int numOfTileX = 0;
    const char* items3[] = {"32", "64", "128", "192", "256" };
    ImGui::Combo("x", &numOfTileX, items3, 5);
    ImGui::NextColumn();
    
    static int numOfTileY = 0;
    const char* items4[] = {"32", "64", "128", "192", "256" };
    ImGui::Combo("y", &numOfTileY, items4, 5);
    
    ImGui::EndChild();
    ImGui::PopStyleVar();
    
    ImGui::Text("World size is (%d) x (%d)",
                atoi(items1[tileSizeXItem]) * atoi(items3[numOfTileX]),
                atoi(items2[tileSizeYItem]) * atoi(items4[numOfTileY]));
    
    ImGui::Text("\n");
    
    ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
    ImGui::BeginChild("##Default Tile", ImVec2(0,180), true);
    
    ImGui::Text("Default Tile");
    
    const char* tiles[] = { "Dirt", "Grass", "Water", "Hill" };
    static int currentTile = 0;
    ImGui::ListBox("select\ndefault tile", &currentTile, tiles, 4, 6);
    
    ImGui::EndChild();
    ImGui::PopStyleVar();
    
    if (ImGui::Button("Create"))
    {
        // create map
        
        GMXFile* file = new GMXFile();
        
        static int nextNumber = 0;
        file->fileName = "untitled_map_" + to_string(nextNumber++);
        file->tileWidth = atoi(items1[tileSizeXItem]);
        file->tileHeight = atoi(items2[tileSizeYItem]);
        file->numOfTileX = atoi(items3[numOfTileX]);
        file->numOfTileY = atoi(items4[numOfTileY]);
        file->worldSize = Size(file->tileWidth * file->numOfTileX, file->tileHeight * file->numOfTileY);
        file->defaultTile = currentTile;
        
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
                
                if ( currentTile == TileType::DIRT) tileName = "1_" + std::to_string(random(1, 3)) + "_1234";
                else if ( currentTile == TileType::GRASS) tileName = "2_" + std::to_string(random(1, 3)) + "_1234";
                else if ( currentTile == TileType::WATER) tileName = "3_" + std::to_string(random(1, 3)) + "_1234";
                else if ( currentTile == TileType::HILL) tileName = "5_" + std::to_string(random(1, 3)) + "_1234";
                
                file->tileInfos[i][j] = tileName;
            }
        }
        
        auto newLayer = GMXLayer::create();
        newLayer->openFile(file);
        
        _palette = PaletteWindow::create();
        newLayer->addChild(_palette);
        
        _triggerEditor = TriggerEditor::create();
        newLayer->addChild(_triggerEditor);
        
        _gmxLayerManager->addChild(newLayer);
        _minimapLayer->setGMXLayer(newLayer);
        
        _isEditEnable = true;
        _isPlayerEnable = true;
        _isWindowEnable = true;
        _showFileMenuBar = true;
        _showPalette = true;
        _isFileEnable = true;
        
        tileSizeXItem = 0;
        tileSizeYItem = 0;
        numOfTileX = 0;
        numOfTileY = 0;
        currentTile = 0;
        *opened = false;
    }
    ImGui::SameLine();
    if ( ImGui::Button("Cancel") )
    {
        _isFileEnable = true;
        
        tileSizeXItem = 0;
        tileSizeYItem = 0;
        numOfTileX = 0;
        numOfTileY = 0;
        currentTile = 0;
        *opened = false;
    }
    
    if (*opened == false)
    {
        _isFileEnable = true;
        
        tileSizeXItem = 0;
        tileSizeYItem = 0;
        numOfTileX = 0;
        numOfTileY = 0;
        currentTile = 0;
    }
    
    ImGui::End();
}


void HelloWorld::showFileMenuBar(bool* opened)
{
    ImGui::SetNextWindowPos(ImVec2(MINIMAP_SIZE + WINDOW_PADDING * 2, MENUBAR_HEIGHT + WINDOW_PADDING));
    ImGui::SetNextWindowSize(ImVec2(_oldWindowSize.width - MINIMAP_SIZE - WINDOW_PADDING * 3, FILE_MENUBAR_HEIGHT));
    
    std::string fileName = _gmxLayerManager->getCurrentLayer()->getFileName() + ".gmx";
    if ( _gmxLayerManager->getCurrentLayer()->isChanged() )
    {
        fileName += " *";
    }
    
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.8200000, 0.8200000, 0.8200000, 1.0000000));
    ImGui::Begin(fileName.c_str(), &_showFileMenuBar, ImVec2(0,0), 0.0f,
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoBringToFrontOnFocus);
    
    ImGui::End();
    ImGui::PopStyleColor(1);
    
    if ( *opened == false )
    {
        // closed
        _showPalette = false;
        _showTrigger = false;
        _isRedo = false;
        _isWindowEnable = false;
        _isPlayerEnable = false;
        _isEditEnable = false;
        _isUndo = false;
        Dispatch.removeNode(MessageNodeType::GMX_LAYER);
        _viewSpaceParams.setZero();
        _worldPosition = Vec2::ZERO;
        _minimapLayer->disableFocusWindow();
        _gmxLayerManager->closeLayer();
    }
}



void HelloWorld::redo()
{
    if ( _gmxLayerManager->getCurrentLayer() && _isRedo )
    {
        _gmxLayerManager->getCurrentLayer()->redo();
    }
}


void HelloWorld::undo()
{
    if ( _gmxLayerManager->getCurrentLayer() && _isUndo )
    {
        _gmxLayerManager->getCurrentLayer()->undo();
    }
}


void HelloWorld::save()
{
    if ( _gmxLayerManager->getCurrentLayer() )
    {
        _gmxLayerManager->saveGMXFile("test");
        log("Save");
    }
}


void HelloWorld::open()
{
    GMXFile* file = new GMXFile();
    
    _gmxLayerManager->loadGMXFile(file, "test");
    auto newLayer = GMXLayer::create();
    newLayer->openFile(file);
    
    _palette = PaletteWindow::create();
    newLayer->addChild(_palette);
    
    _gmxLayerManager->addChild(newLayer);
    _minimapLayer->setGMXLayer(newLayer);
    
    _isEditEnable = true;
    _isPlayerEnable = true;
    _isWindowEnable = true;
    _showFileMenuBar = true;
    _showPalette = true;
}










