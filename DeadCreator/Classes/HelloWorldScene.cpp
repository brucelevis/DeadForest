#include "HelloWorldScene.h"
#include "CCImGui.h"
USING_NS_CC;

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    CCIMGUI->addImGUI([=](){
        
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
    
    
    CCIMGUI->addImGUI([=]{
        
        ImGuiIO io = ImGui::GetIO();
        ImGuiStyle style = ImGui::GetStyle();
        
        ImGui::SetNextWindowPos(ImVec2(0.0f, io.DisplaySize.y - ImGui::GetWindowFontSize() - style.FramePadding.y * 4.0f));
        ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, ImGui::GetWindowFontSize() + style.FramePadding.y));
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
    
    CCIMGUI->addImGUI([=]{
        
        ImGuiIO io = ImGui::GetIO();
        ImGuiStyle style = ImGui::GetStyle();
        
        ImGui::SetNextWindowPos(ImVec2(5.0f, 210 + ImGui::GetWindowFontSize() + style.FramePadding.y * 2.0f));
        ImGui::SetNextWindowSize(ImVec2(200, io.DisplaySize.y - 220 - ImGui::GetWindowFontSize() * 2 - style.FramePadding.y * 4.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        if (ImGui::Begin(" Entities", NULL,
                         ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoBringToFrontOnFocus |
                         ImGuiWindowFlags_NoMove))
        {
            // left
            static int selected = 0;
            ImGui::BeginChild("left pane", ImVec2(180, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
            for (int i = 0; i < 1; i++)
            {
                char label[128];
                sprintf(label, "MyObject %d", i);
                if (ImGui::Selectable(label, selected == i))
                    selected = i;
            }
            ImGui::EndChild();
        }
        ImGui::End();
        ImGui::PopStyleVar(1);
        
    }, "property_menu");
    
    
    CCIMGUI->addImGUI([=]{
        
        ImGuiStyle style = ImGui::GetStyle();
        
        ImGui::SetNextWindowPos(ImVec2(5, ImGui::GetWindowFontSize() + style.FramePadding.y * 2.0f + 5));
        ImGui::SetNextWindowSize(ImVec2(200, 200));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::Begin("##Minimap", NULL,
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoBringToFrontOnFocus |
                     ImGuiWindowFlags_NoMove);
        ImGui::End();
        ImGui::PopStyleVar(1);
        
    }, "mini_map");
    
    CCIMGUI->addImGUI([=]{
    
        ImGui::SetNextWindowPos(ImVec2(750, 100), ImGuiWindowFlags_NoResize);
        ImGui::SetNextWindowSize(ImVec2(200,300));
        ImGui::Begin("Palette", NULL);
        static int item = 0;
        ImGui::Combo("type", &item, "tile\0entity\0doodad");
        ImGui::End();
        
    }, "palette_window");
    
    CCIMGUI->addImGUI([=]{
        
        static bool isShowDemo = true;
        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
        ImGui::ShowTestWindow(&isShowDemo);
        
        ImGui::SetNextWindowPos(ImVec2(300, 300), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("##Button", NULL, ImVec2(300,300), 0.0f);
        CCIMGUI->imageButton("CloseNormal.png");
        ImGui::End();

        
    }, "test_window");

    
    auto spr = Sprite::create("HelloWorld.png");
    spr->setPosition(Vec2(568,320));
    addChild(spr);
    
    this->scheduleUpdate();
    
    return true;
}


void HelloWorld::update(float dt)
{
    // log("%f %f", _director->getVisibleSize().width, _director->getVisibleSize().height);
}









