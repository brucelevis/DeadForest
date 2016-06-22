//
//  PaletteWindow.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 22..
//
//

#include "PaletteWindow.hpp"


PaletteWindow::PaletteWindow()
{
}


PaletteWindow::~PaletteWindow()
{
}


PaletteWindow* PaletteWindow::create()
{
    auto ret = new (std::nothrow) PaletteWindow();
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_FREE(ret);
    return nullptr;
}


void PaletteWindow::showPaletteWindow(bool* opened)
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
        CCIMGUI->imageButton("1_1_1234.png", 50, 50); ImGui::SameLine();
        CCIMGUI->imageButton("2_1_1234.png", 50, 50); ImGui::SameLine();
        CCIMGUI->imageButton("3_1_1234.png", 50, 50);
        CCIMGUI->imageButton("5_1_1234.png", 50, 50);
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



