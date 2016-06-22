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
    ImGui::SetNextWindowPos(ImVec2(750, 100), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiSetCond_Once);
    
    ImGui::Begin("Palette", opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    
    if (ImGui::Combo("type", &_paletteType, "tile\0entity\0item\0doodad\0"))
    {
        _selectedItem = -1;
    }
    
    ImGui::Separator();
    
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35, 0.35, 0.35, 0.35));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35, 0.35, 0.35, 0.55));
    
    if ( _paletteType == 0)
    {
        if (CCIMGUI->imageButton("1_1_1234.png", 50, 50)) _selectedItem = 0;
        ImGui::SameLine();
        if (CCIMGUI->imageButton("2_1_1234.png", 50, 50)) _selectedItem = 1;
        ImGui::SameLine();
        if (CCIMGUI->imageButton("3_1_1234.png", 50, 50)) _selectedItem = 2;
        
        if (CCIMGUI->imageButton("5_1_1234.png", 50, 50)) _selectedItem = 3;
    }
    
    else if ( _paletteType == 1)
    {
        if (CCIMGUI->imageButton("human.png", 50, 50)) _selectedItem = 0;
    }
    
    else if ( _paletteType == 2)
    {
        if (CCIMGUI->imageButton("5_56mm.png", 50, 50)) _selectedItem = 0;
        ImGui::SameLine();
        if (CCIMGUI->imageButton("9mm.png", 50, 50)) _selectedItem = 1;
        ImGui::SameLine();
        if (CCIMGUI->imageButton("Shell.png", 50, 50)) _selectedItem = 2;
        
        if (CCIMGUI->imageButton("Axe.png", 50, 50)) _selectedItem = 3;
        ImGui::SameLine();
        if (CCIMGUI->imageButton("Glock17.png", 50, 50)) _selectedItem = 4;
        ImGui::SameLine();
        if (CCIMGUI->imageButton("M16A2.png", 50, 50)) _selectedItem = 5;
        
        if (CCIMGUI->imageButton("M1897.png", 50, 50)) _selectedItem = 6;
        ImGui::SameLine();
        if (CCIMGUI->imageButton("MeatCan.png", 50, 50)) _selectedItem = 7;
        ImGui::SameLine();
        if (CCIMGUI->imageButton("Bandage.png", 50, 50)) _selectedItem = 8;
    }
    ImGui::PopStyleColor(3);
    
    ImGui::End();
}



