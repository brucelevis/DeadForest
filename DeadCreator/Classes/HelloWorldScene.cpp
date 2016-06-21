#include <fstream>
using namespace std;

#include <boost/filesystem.hpp>

#include "HelloWorldScene.h"
#include "GMXLayer.hpp"
#include "CCImGui.h"
using namespace cocos2d;


HelloWorld::HelloWorld() :
_workSpaceSize(Size::ZERO)
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
    
    ImGuiStyle& style = ImGui::GetStyle();
    
    float fontBaseSize = 25.0f;
    _firstDisplaySize.setSize(_director->getVisibleSize().width, _director->getVisibleSize().height);
    _minimapSize.setSize(_firstDisplaySize.width * 0.15f, _firstDisplaySize.width * 0.15f);
    _menuBarHeight = fontBaseSize;
    _statusBarHeight = fontBaseSize + style.FramePadding.y * 2.0f;
    
    CCIMGUI->addImGUI([this](){
        
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::MenuItem("(dummy menu)", NULL, false, false);
                if (ImGui::MenuItem("New")) {}
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
                if (ImGui::MenuItem("Trigger Editor", "SHIFT+T")) {}
                if (ImGui::MenuItem("Palette Window", "SHIFT+P")) {}
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
        
        ImGui::SetNextWindowPos(ImVec2(0.0f, io.DisplaySize.y - _statusBarHeight));
        ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, _statusBarHeight));
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
        
        ImGui::SetNextWindowPos(ImVec2(WINDOW_PADDING, _menuBarHeight + _minimapSize.height + WINDOW_PADDING * 2));
        ImGui::SetNextWindowSize(ImVec2(_minimapSize.width, io.DisplaySize.y - (_menuBarHeight + _minimapSize.height + _statusBarHeight + WINDOW_PADDING * 3)));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        if (ImGui::Begin(" Entities", NULL,
                         ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoBringToFrontOnFocus |
                         ImGuiWindowFlags_NoMove))
        {
            static int selected = 0;
            ImGui::BeginChild("##ObjectList", ImVec2(_minimapSize.width - style.WindowPadding.x * 2, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
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
        
    }, "palette_window");
    
    
    CCIMGUI->addImGUI([this]{
        
        static bool isShowDemo = true;
        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
        ImGui::ShowTestWindow(&isShowDemo);

    }, "test_window");
    
    
    _gmxLayer = GMXLayer::create("test.txt");
    _gmxLayer->setPosition(Vec2(_minimapSize.width + WINDOW_PADDING * 2, _statusBarHeight + WINDOW_PADDING));
    addChild(_gmxLayer);
    
    _workSpaceSize = _gmxLayer->getWorldSize();
    
    _viewSpaceSize.setSize(_director->getVisibleSize().width - _minimapSize.width - WINDOW_PADDING * 3, _director->getVisibleSize().height - _menuBarHeight - WINDOW_PADDING * 2 - _statusBarHeight);
    _centerPosition = _viewSpaceSize / 2;
    _viewSpaceParams.setPoint((_centerPosition.x - _viewSpaceSize.width / 2) / (_workSpaceSize.width - _viewSpaceSize.width),
                              (_centerPosition.y - _viewSpaceSize.height / 2) / (_workSpaceSize.height - _viewSpaceSize.height));

    _minimapFocusWindowSize.setSize(_minimapSize.width * (_viewSpaceSize.width/_workSpaceSize.width), _minimapSize.height * (_viewSpaceSize.height / _workSpaceSize.height));
    
    _minimapRoot = Node::create();
    _minimapRoot->setPosition(Vec2(WINDOW_PADDING, _director->getVisibleSize().height - _menuBarHeight - WINDOW_PADDING - _minimapSize.height));
    addChild(_minimapRoot);
    
    _minimapBG = Sprite::create("bg.png");
    _minimapBG->setAnchorPoint(Vec2::ZERO);
    _minimapBG->setScale(_minimapSize.width / _minimapBG->getContentSize().width, _minimapSize.height / _minimapBG->getContentSize().height);
    _minimapRoot->addChild(_minimapBG);
    
    _minimapFocusWindow = DrawNode::create();
    _minimapFocusWindow->drawRect(Vec2(-_minimapFocusWindowSize.width / 2, -_minimapFocusWindowSize.height / 2),
                                  Vec2(_minimapFocusWindowSize.width/2, _minimapFocusWindowSize.height/2),
                                  Color4F::WHITE);
    _minimapRoot->addChild(_minimapFocusWindow);
    
    this->scheduleUpdate();
    
//    GMXFile* file = new GMXFile;
//    GMXFileManager manager(file);
//    if ( manager.loadGMXFile("test.txt") ) log("suc");
//    else log("fail");
    
    
    return true;
}


void HelloWorld::update(float dt)
{
    static auto director = Director::getInstance();
    Size currSize = director->getVisibleSize();
    if ( _oldWindowSize.width != currSize.width || _oldWindowSize.height != currSize.height)
    {
        // resize
        _viewSpaceSize.setSize(currSize.width - _minimapSize.width - WINDOW_PADDING * 3, currSize.height - _menuBarHeight - WINDOW_PADDING * 2 - _statusBarHeight);
        _minimapRoot->setPosition(Vec2(WINDOW_PADDING, currSize.height - _menuBarHeight - WINDOW_PADDING - _minimapSize.height));
        _minimapBG->setScale(_minimapSize.width / _minimapBG->getContentSize().width, _minimapSize.height / _minimapBG->getContentSize().height);
        _minimapFocusWindowSize.setSize(_minimapSize.width * (_viewSpaceSize.width/_workSpaceSize.width), _minimapSize.height * (_viewSpaceSize.height / _workSpaceSize.height));
        
        _minimapFocusWindow->clear();
        _minimapFocusWindow->drawRect(Vec2(-_minimapFocusWindowSize.width / 2, -_minimapFocusWindowSize.height / 2),
                                      Vec2(_minimapFocusWindowSize.width/2, _minimapFocusWindowSize.height/2),
                                      Color4F::WHITE);
    }
    
    _minimapFocusWindow->setPosition(Vec2(_minimapFocusWindowSize.width / 2 +  _viewSpaceParams.x * (_minimapSize.width - _minimapFocusWindowSize.width),
                                          _minimapFocusWindowSize.height / 2 + _viewSpaceParams.y * (_minimapSize.height - _minimapFocusWindowSize.height)));
    
    _oldWindowSize = director->getVisibleSize();
    
    if ( _isKeyPressed[static_cast<int>(EventKeyboard::KeyCode::KEY_LEFT_ARROW)] == true )
    {
        _viewSpaceParams.x = _viewSpaceParams.x - (1000/_workSpaceSize.width) * dt;
        _viewSpaceParams.x = clampf(_viewSpaceParams.x, 0.0f, 1.0f);
    }
    else if ( _isKeyPressed[static_cast<int>(EventKeyboard::KeyCode::KEY_RIGHT_ARROW)] == true )
    {
        _viewSpaceParams.x = _viewSpaceParams.x + (1000/_workSpaceSize.height) * dt;
        _viewSpaceParams.x = clampf(_viewSpaceParams.x, 0.0f, 1.0f);
    }
    
    if ( _isKeyPressed[static_cast<int>(EventKeyboard::KeyCode::KEY_DOWN_ARROW)] == true )
    {
        _viewSpaceParams.y = _viewSpaceParams.y - (1000/_workSpaceSize.width) * dt;
        _viewSpaceParams.y = clampf(_viewSpaceParams.y, 0.0f, 1.0f);
    }
    else if ( _isKeyPressed[static_cast<int>(EventKeyboard::KeyCode::KEY_UP_ARROW)] == true )
    {
        _viewSpaceParams.y = _viewSpaceParams.y + (1000/_workSpaceSize.height) * dt;
        _viewSpaceParams.y = clampf(_viewSpaceParams.y, 0.0f, 1.0f);
    }

//    _tileRoot->setPosition(Vec2(-(_workSpaceSize.width - _viewSpaceSize.width) * _viewSpaceParams.x, -(_workSpaceSize.height - _viewSpaceSize.height) * _viewSpaceParams.y));
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

    Rect minimapRect(_minimapRoot->getPosition().x, _minimapRoot->getPosition().y, _minimapSize.width, _minimapSize.height);
    
    if ( minimapRect.containsPoint(_mousePosition) )
    {
        Vec2 innerPosition = _mousePosition - _minimapRoot->getPosition();
        _viewSpaceParams.setPoint((innerPosition.x - _minimapFocusWindowSize.width / 2) / (_minimapSize.width - _minimapFocusWindowSize.width),
                                  (innerPosition.y - _minimapFocusWindowSize.height / 2) / (_minimapSize.height - _minimapFocusWindowSize.height));
        
        _viewSpaceParams.x = clampf(_viewSpaceParams.x, 0.0, 1.0);
        _viewSpaceParams.y = clampf(_viewSpaceParams.y, 0.0, 1.0);
    }
}

void HelloWorld::onMouseMove(cocos2d::Event* event)
{
    if ( _isMousePressed )
    {
        auto mouseEvent = static_cast<EventMouse*>(event);
        _mousePosition.setPoint(mouseEvent->getCursorX(), mouseEvent->getCursorY());
        
        Rect minimapRect(_minimapRoot->getPosition().x, _minimapRoot->getPosition().y, _minimapSize.width, _minimapSize.height);
        
        if ( minimapRect.containsPoint(_mousePosition) )
        {
            Vec2 innerPosition = _mousePosition - _minimapRoot->getPosition();
            _viewSpaceParams.setPoint((innerPosition.x - _minimapFocusWindowSize.width / 2) / (_minimapSize.width - _minimapFocusWindowSize.width),
                                      (innerPosition.y - _minimapFocusWindowSize.height / 2) / (_minimapSize.height - _minimapFocusWindowSize.height));
            
            _viewSpaceParams.x = clampf(_viewSpaceParams.x, 0.0, 1.0);
            _viewSpaceParams.y = clampf(_viewSpaceParams.y, 0.0, 1.0);
        }
    }
}

void HelloWorld::onMouseUp(cocos2d::Event* event)
{
    _isMousePressed = false;
}














