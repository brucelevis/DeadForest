#include <fstream>
using namespace std;

#include "HelloWorldScene.h"
#include "CCImGui.h"
using namespace cocos2d;


HelloWorld::HelloWorld() :
_workSpaceSize(Size::ZERO),
_tileWidth(128),
_tileHeight(128)
{
}


bool HelloWorld::init()
{
    if ( !LayerColor::initWithColor(Color4B::BLACK) )
    {
        return false;
    
    }
    
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
        
        ImGui::SetNextWindowPos(ImVec2(WINDOW_PADDING, _menuBarHeight + WINDOW_PADDING));
        ImGui::SetNextWindowSize(ImVec2(_minimapSize.width, _minimapSize.height));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::Begin("##Minimap", NULL,
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoBringToFrontOnFocus |
                     ImGuiWindowFlags_NoMove);
        ImGui::End();
        ImGui::PopStyleVar(1);
        
    }, "mini_map");
    
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

    
    _scrollBarRenderer = DrawNode::create();
    addChild(_scrollBarRenderer);
    
    loadMap("testmap");
    
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
        _tileRoot->setClippingRegion(Rect(0, 0, currSize.width - (WINDOW_PADDING * 3 + _minimapSize.width + SCROLL_BAR_HEIGHT), currSize.height - (_menuBarHeight + _statusBarHeight + WINDOW_PADDING * 2 + SCROLL_BAR_HEIGHT)));
    }
    
    _scrollBarRenderer->clear();
    _scrollBarRenderer->drawRect(Vec2(_minimapSize.width + WINDOW_PADDING * 2, _statusBarHeight + WINDOW_PADDING),
                         Vec2(_oldWindowSize.width - WINDOW_PADDING - SCROLL_BAR_HEIGHT, _statusBarHeight + WINDOW_PADDING + SCROLL_BAR_HEIGHT),
                         Color4F(0.5, 0.5, 0.5, 0.4));
    
    _scrollBarRenderer->drawRect(Vec2(_oldWindowSize.width - (WINDOW_PADDING + SCROLL_BAR_HEIGHT), _statusBarHeight + WINDOW_PADDING + SCROLL_BAR_HEIGHT),
                         Vec2(_oldWindowSize.width - WINDOW_PADDING, _oldWindowSize.height - (_menuBarHeight + WINDOW_PADDING)),
                         Color4F(0.5, 0.5, 0.5, 0.4));
    
    _oldWindowSize = director->getVisibleSize();
}


void HelloWorld::loadMap(const std::string& fileName)
{
//    ifstream fin(fileName);
//    int x, y;
//    fin >> x >> y;
//    log("%d %d", x, y);
//    fin.close();
    
    Size currSize = Director::getInstance()->getVisibleSize();
    
    _tileRoot = ClippingRectangleNode::create(Rect(0, 0, currSize.width - (WINDOW_PADDING * 3 + _minimapSize.width + SCROLL_BAR_HEIGHT), currSize.height - (_menuBarHeight + _statusBarHeight + WINDOW_PADDING * 2 + SCROLL_BAR_HEIGHT)));
    _tileRoot->setPosition(Vec2(_minimapSize.width + WINDOW_PADDING * 2, _statusBarHeight + WINDOW_PADDING + SCROLL_BAR_HEIGHT));
    addChild(_tileRoot);
    
    _tileHeight = 60;
    _tileWidth = 30;
    
    _tileImages.resize(_tileHeight);
    for(int i = 0 ; i < _tileHeight ; ++ i)
    {
        _tileImages[i].resize(_tileWidth);
    }
    
    _workSpaceSize = Size(_tileWidth * 128, _tileHeight * 128);
    for(int i = 0 ; i < _tileHeight ; ++ i)
    {
        for(int j = 0 ; j < _tileWidth ; ++ j)
        {
            Vec2 tilePosition;
            if ( i % 2 == 0)
            {
                tilePosition.setPoint(j * 128, i * 64);
            }
            else
            {
                tilePosition.setPoint(64 + j * 128, i * 64);
            }
            if ( rand() % 2  == 0)
            {
                _tileImages[i][j] = Sprite::create("dirt.png");
            }
            else
            {
                _tileImages[i][j] = Sprite::create("grass.png");
            }
            _tileImages[i][j]->setPosition(tilePosition);
            _tileRoot->addChild(_tileImages[i][j]);
        }
    }
}









