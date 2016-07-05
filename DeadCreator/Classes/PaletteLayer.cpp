//
//  PaletteLayer.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 22..
//
//

#include "PaletteLayer.hpp"
#include "ImGuiLayer.h"
#include "GMXLayer2.hpp"
#include "TileToolCommand.hpp"
#include "EntityToolCommand.hpp"
using namespace cocos2d;

PaletteLayer::PaletteLayer(GMXLayer2& layer):
_gmxLayer(layer),
_layerSize(Size(200,200)),
_layerPosition(Vec2(200, 100)),
_boundingBoxPadding(Rect::ZERO)
{
}


PaletteLayer::~PaletteLayer()
{
}


PaletteLayer* PaletteLayer::create(GMXLayer2& layer)
{
    auto ret = new (std::nothrow) PaletteLayer(layer);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_FREE(ret);
    return nullptr;
}


void PaletteLayer::showLayer(bool* opened)
{
    ImGuiState& g = *GImGui;
    float height = g.FontBaseSize + g.Style.FramePadding.y * 2.0f;
    
    if ( _layerPosition.x < WINDOW_PADDING )
    {
        _layerPosition.x = WINDOW_PADDING;
    }
    
    if ( _layerPosition.y < height + WINDOW_PADDING + ICONBAR_HEIGHT )
    {
        _layerPosition.y = height + WINDOW_PADDING + ICONBAR_HEIGHT;
    }
    
    if ( _layerPosition.x + _layerSize.width > g.IO.DisplaySize.x - WINDOW_PADDING )
    {
        _layerPosition.x = g.IO.DisplaySize.x - _layerSize.width - WINDOW_PADDING;
    }
    
    if ( _layerPosition.y + _layerSize.height > g.IO.DisplaySize.y - WINDOW_PADDING - STATUSBAR_HEIGHT )
    {
        _layerPosition.y = g.IO.DisplaySize.y - _layerSize.height - WINDOW_PADDING - STATUSBAR_HEIGHT;
    }
  
    
    ImGui::SetNextWindowPos(ImVec2(_layerPosition.x, _layerPosition.y), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(205, 300), ImGuiSetCond_Once);
    
    ImGui::Begin("Palette", opened, ImVec2(0,0), 0.9f,
                 ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoScrollWithMouse |
                 ImGuiWindowFlags_ShowBorders);

    
    _layerPosition.setPoint(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
    _layerSize.setSize(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
    
    _boundingBoxPadding.setRect(_layerPosition.x + g.Style.WindowPadding.x,
                                _layerPosition.y + g.Style.WindowPadding.y + height,
                                _layerSize.width - g.Style.WindowPadding.x * 2.0f,
                                _layerSize.height - g.Style.WindowPadding.y * 2.0f - height);
    
    
    if (ImGui::Combo("type", &_paletteType, "tile\0human\0item\0doodad\0"))
    {
        _selectedItem = 0;
    }
    
    ImGui::Separator();
    
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35, 0.35, 0.35, 0.35));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35, 0.35, 0.35, 0.55));
    
    if ( _paletteType == 0)
    {
        _gmxLayer.setCommand(_gmxLayer.getTileToolCommand());
        
        if ( ImGuiLayer::imageButton("1_1_1234.png", 50, 50) )
        {
            _selectedItem = static_cast<int>(TileType::DIRT);
        }
        ImGui::SameLine();
        
        if ( ImGuiLayer::imageButton("2_1_1234.png", 50, 50) )
        {
            _selectedItem = static_cast<int>(TileType::GRASS);
        }
        ImGui::SameLine();
        
        if ( ImGuiLayer::imageButton("3_1_1234.png", 50, 50) )
        {
            _selectedItem = static_cast<int>(TileType::WATER);
        }
        
        if ( ImGuiLayer::imageButton("5_1_1234.png", 50, 50) )
        {
            _selectedItem = static_cast<int>(TileType::HILL);
        }
    }
    
    else if ( _paletteType == 1)
    {
        _gmxLayer.setCommand(_gmxLayer.getEntityToolCommand());
        
        if ( ImGuiLayer::imageButton("human.png", 50, 50) )
        {
            _selectedItem = static_cast<int>(EntityType::SHERIFF);
        }
    }
    
    else if ( _paletteType == 2)
    {
        _gmxLayer.setCommand(_gmxLayer.getEntityToolCommand());
        
        if (ImGuiLayer::imageButton("5_56mm.png", 50, 50)) _selectedItem = 0;
        ImGui::SameLine();
        if (ImGuiLayer::imageButton("9mm.png", 50, 50)) _selectedItem = 1;
        ImGui::SameLine();
        if (ImGuiLayer::imageButton("Shell.png", 50, 50)) _selectedItem = 2;
        
        if (ImGuiLayer::imageButton("Axe.png", 50, 50)) _selectedItem = 3;
        ImGui::SameLine();
        if (ImGuiLayer::imageButton("Glock17.png", 50, 50)) _selectedItem = 4;
        ImGui::SameLine();
        if (ImGuiLayer::imageButton("M16A2.png", 50, 50)) _selectedItem = 5;
        
        if (ImGuiLayer::imageButton("M1897.png", 50, 50)) _selectedItem = 6;
        ImGui::SameLine();
        if (ImGuiLayer::imageButton("MeatCan.png", 50, 50)) _selectedItem = 7;
        ImGui::SameLine();
        if (ImGuiLayer::imageButton("Bandage.png", 50, 50)) _selectedItem = 8;
    }
    
    ImGui::PopStyleColor(3);
    ImGui::End();
}



