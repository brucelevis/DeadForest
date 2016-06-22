#include <fstream>
using namespace std;

#include <boost/filesystem.hpp>
#include <boost/any.hpp>

#include "HelloWorldScene.h"
#include "GMXLayer.hpp"
#include "CCImGui.h"
#include "SizeProtocol.h"
using namespace cocos2d;


HelloWorld::HelloWorld()
{
}


bool HelloWorld::init()
{
    if ( !LayerColor::initWithColor(Color4B::BLACK) )
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
    
    // init();
    CCImGui::getInstance();
    
    
    CCIMGUI->addImGUI([this](){
        
        if (_showNewMap) showNewMapWindow(&_showNewMap);
        if (_showPalette) showPaletteWindow(&_showPalette);
        
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::MenuItem("(dummy menu)", NULL, false, false);
                if (ImGui::MenuItem("New", NULL, &_showNewMap))
                {
                    if ( _showNewMap ) _isPaletteEnable = true;
                    else _isPaletteEnable = false;
                }
                
                if (ImGui::MenuItem("Open", "Ctrl+O")) {}
                if (ImGui::BeginMenu("Open Recent"))
                {
                    ImGui::MenuItem("fish_hat.c");
                    ImGui::MenuItem("fish_hat.inl");
                    ImGui::MenuItem("fish_hat.h");
                    if (ImGui::BeginMenu("More.."))
                    {
                        ImGui::MenuItem("Hello");
                        ImGui::MenuItem("Sailor");
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {}
                if (ImGui::MenuItem("Save As..")) {}
                ImGui::Separator();
                if (ImGui::BeginMenu("Options"))
                {
                    static bool enabled = true;
                    ImGui::MenuItem("Enabled", "", &enabled);
                    ImGui::BeginChild("child", ImVec2(0, 60), true);
                    for (int i = 0; i < 10; i++)
                        ImGui::Text("Scrolling Text %d", i);
                    ImGui::EndChild();
                    static float f = 0.5f;
                    static int n = 0;
                    ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
                    ImGui::InputFloat("Input", &f, 0.1f);
                    ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Colors"))
                {
                    for (int i = 0; i < ImGuiCol_COUNT; i++)
                        ImGui::MenuItem(ImGui::GetStyleColName((ImGuiCol)i));
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Disabled", false)) // Disabled
                {
                    IM_ASSERT(0);
                }
                if (ImGui::MenuItem("Checked", NULL, true)) {}
                if (ImGui::MenuItem("Quit", "Alt+F4")) {}
                
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Players"))
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
            
            if (ImGui::BeginMenu("Windows"))
            {
                ImGui::MenuItem("Trigger Editor", "SHIFT+T");
                ImGui::MenuItem("Palette Window", "SHIFT+P", &_showPalette, _isPaletteEnable);
                ImGui::Separator();
                if (ImGui::MenuItem("test.gmx")) {}
                if (ImGui::MenuItem("test1.gmx")) {}
                if (ImGui::MenuItem("test2.gmx")) {}
                
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
        
    }, "main_menu_bar");
    
    
    CCIMGUI->addImGUI([this]{
        
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
        ImGui::Text("MousePosition (%.0f, %.0f)",
                    cocos2d::clampf(ImGui::GetMousePos().x, 0.0f, io.DisplaySize.x),
                    cocos2d::clampf(io.DisplaySize.y - ImGui::GetMousePos().y, 0.0f, io.DisplaySize.y));
        
        ImGui::SameLine();
        ImGui::NextColumn();
        ImGui::Text("Player 1");
        
        ImGui::SameLine();
        ImGui::NextColumn();
        ImGui::Text("Object 372");
        
        ImGui::SameLine();
        ImGui::NextColumn();
        ImGui::Text("Position (%.0f, %.0f)", 11314.0f, 3323.0f);
        
        ImGui::SameLine();
        ImGui::NextColumn();
        ImGui::Text("(%.2f FPS)", ImGui::GetIO().Framerate);
        
        ImGui::End();
        ImGui::PopStyleVar(1);
        
    }, "bottom_menu_bar");
    
    CCIMGUI->addImGUI([this]{
        
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        
        ImGui::SetNextWindowPos(ImVec2(WINDOW_PADDING, MENUBAR_HEIGHT + MINIMAP_SIZE + WINDOW_PADDING * 2));
        ImGui::SetNextWindowSize(ImVec2(MINIMAP_SIZE, io.DisplaySize.y - (MENUBAR_HEIGHT + MINIMAP_SIZE + STATUSBAR_HEIGHT + WINDOW_PADDING * 3)));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
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
        ImGui::PopStyleVar(1);
        
    }, "property_menu");
    
    
    CCIMGUI->addImGUI([this]{
        
        static bool isShowDemo = true;
        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
        ImGui::ShowTestWindow(&isShowDemo);
        
    }, "test_window");
    
    
    _gmxLayerManager = GMXLayerManager::create();
    _gmxLayerManager->setPosition(Vec2(MINIMAP_SIZE + WINDOW_PADDING * 2, STATUSBAR_HEIGHT + WINDOW_PADDING));
    addChild(_gmxLayerManager);
    
    
    _minimapLayer = MinimapLayer::create(Size(MINIMAP_SIZE,MINIMAP_SIZE));
    _minimapLayer->setPosition(Vec2(WINDOW_PADDING, _director->getVisibleSize().height - MENUBAR_HEIGHT - MINIMAP_SIZE - WINDOW_PADDING));
    addChild(_minimapLayer);
    
    this->scheduleUpdate();
    
    return true;
}


void HelloWorld::update(float dt)
{
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
}


void HelloWorld::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    _isKeyPressed[static_cast<int>(keyCode)] = true;
}


void HelloWorld::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    _isKeyPressed[static_cast<int>(keyCode)] = false;
}


void HelloWorld::onMouseDown(cocos2d::Event* event)
{
    _isMousePressed = true;
    
    auto mouseEvent = static_cast<EventMouse*>(event);
    _mousePosition.setPoint(mouseEvent->getCursorX(), mouseEvent->getCursorY());
    
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
}


void HelloWorld::onMouseMove(cocos2d::Event* event)
{
    if ( _isMousePressed )
    {
        auto mouseEvent = static_cast<EventMouse*>(event);
        _mousePosition.setPoint(mouseEvent->getCursorX(), mouseEvent->getCursorY());
        
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
    }
}


void HelloWorld::onMouseUp(cocos2d::Event* event)
{
    _isMousePressed = false;
}


void HelloWorld::onCenterView()
{
    _gmxLayerManager->onCenterView(_viewSpaceParams.x, _viewSpaceParams.y);
    
    if ( _minimapLayer ) _minimapLayer->onCenterView(_viewSpaceParams);
}


void HelloWorld::onResize()
{
    _gmxLayerManager->onResize();
    
    if ( _minimapLayer ) _minimapLayer->onResize();
}


void HelloWorld::createGMXLayer(GMXFile* file)
{
    GMXLayer* newLayer = GMXLayer::create();
    
    newLayer->openFile(file);
    _gmxLayerManager->addLayer(newLayer);
    
    _minimapLayer->setGMXLayer(newLayer);
}


void HelloWorld::saveGMXLayer(GMXFile* file, const std::string fileName)
{
}


void HelloWorld::loadGMXLayer(GMXFile* file, const std::string fileName)
{
}


void HelloWorld::showNewMapWindow(bool* opened)
{
    ImGui::SetNextWindowSize(ImVec2(430,430));
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
        createGMXLayer(file);
        
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
        tileSizeXItem = 0;
        tileSizeYItem = 0;
        numOfTileX = 0;
        numOfTileY = 0;
        currentTile = 0;
        *opened = false;
    }

    
    ImGui::End();
}


void HelloWorld::showPaletteWindow(bool* opened)
{
    ImGui::SetNextWindowPos(ImVec2(750, 100), ImGuiWindowFlags_NoResize);
    ImGui::SetNextWindowSize(ImVec2(200, 300));
    
    static bool open = true;
    ImGui::Begin("Palette", &open, ImGuiWindowFlags_NoCollapse);
    
    
    static int item = 0;
    ImGui::Combo("type", &item, "tile\0entity\0item\0doodad");
    
    ImGui::Separator();
    
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35, 0.35, 0.35, 0.35));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35, 0.35, 0.35, 0.55));
    if ( item == 0)
    {
        CCIMGUI->imageButton("dirt.png", 50, 50); ImGui::SameLine();
        CCIMGUI->imageButton("grass.png", 50, 50); ImGui::SameLine();
        CCIMGUI->imageButton("water.png", 50, 50);
        
        CCIMGUI->imageButton("hill.png", 50, 50);
    }
    
    else if ( item == 1)
    {
        CCIMGUI->imageButton("human.png", 50, 50);
    }
    
    else if ( item == 2)
    {
        CCIMGUI->imageButton("5_56mm.png", 50, 50); ImGui::SameLine();
        CCIMGUI->imageButton("9mm.png", 50, 50); ImGui::SameLine();
        CCIMGUI->imageButton("Shell.png", 50, 50);
        
        CCIMGUI->imageButton("Axe.png", 50, 50); ImGui::SameLine();
        CCIMGUI->imageButton("Glock17.png", 50, 50); ImGui::SameLine();
        CCIMGUI->imageButton("M16A2.png", 50, 50);
        
        CCIMGUI->imageButton("M1897.png", 50, 50); ImGui::SameLine();
        CCIMGUI->imageButton("MeatCan.png", 50, 50); ImGui::SameLine();
        CCIMGUI->imageButton("Bandage.png", 50, 50);
    }
    ImGui::PopStyleColor(3);
    
    ImGui::End();
}








