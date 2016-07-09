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
#include "AddEntityToolCommand.hpp"
using namespace realtrick;
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
    ImGuiContext& g = *GImGui;
    float height = g.FontBaseSize + g.Style.FramePadding.y * 2.0f;
    
    ImGui::SetNextWindowPos(ImVec2(_layerPosition.x, _layerPosition.y), ImGuiSetCond_Once);
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
    
    static Vec2 mousePosInCocos2dMatrix;
    mousePosInCocos2dMatrix = Vec2(ImGui::GetIO().MousePos.x, ImGui::GetIO().DisplaySize.y - ImGui::GetIO().MousePos.y);
    
    if ( ImGui::IsMouseHoveringWindow() && ImGui::GetIO().MouseClicked[0] )
    {
        Rect boundingBox(_layerPosition.x, ImGui::GetIO().DisplaySize.y - _layerSize.height - _layerPosition.y, _layerSize.width, _layerSize.height);
        if ( boundingBox.containsPoint(mousePosInCocos2dMatrix) )
        {
            GMXLayer2::enableTitleClicked();
        }
    }
    
    bool itemSelected = false;
    
    if (ImGui::Combo("type", &_paletteType, "tile\0human\0item\0doodad\0"))
    {
        itemSelected = false;
        _selectedItem = -1;
    }
    
    ImGui::Separator();
    
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35, 0.35, 0.35, 0.35));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35, 0.35, 0.35, 0.55));
    
    if ( _paletteType == 0)
    {
        if ( ImGuiLayer::imageButton("1_1_1234.png", 50, 50) )
        {
            _selectedItem = static_cast<int>(EditorTileType::DIRT);
            itemSelected = true;
        }
        ImGui::SameLine();
        
        if ( ImGuiLayer::imageButton("2_1_1234.png", 50, 50) )
        {
            _selectedItem = static_cast<int>(EditorTileType::GRASS);
            itemSelected = true;
        }
        ImGui::SameLine();
        
        if ( ImGuiLayer::imageButton("3_1_1234.png", 50, 50) )
        {
            _selectedItem = static_cast<int>(EditorTileType::WATER);
            itemSelected = true;
        }
        
        if ( ImGuiLayer::imageButton("5_1_1234.png", 50, 50) )
        {
            _selectedItem = static_cast<int>(EditorTileType::HILL);
            itemSelected = true;
        }
        
        if ( itemSelected )
        {
            _gmxLayer.setCommand(_gmxLayer.getTileToolCommand());
            _gmxLayer.enableEntityBoundingBoxNode(false);
        }
    }
    
    else if ( _paletteType == 1)
    {
        if ( ImGuiLayer::imageButton("human.png", 50, 50) )
        {
            _selectedItem = static_cast<int>(EditorEntityType::SHERIFF);
            itemSelected = true;
        }
        
        if ( itemSelected )
        {
            _gmxLayer.setCommand(_gmxLayer.getAddEntityToolCommand());
            _gmxLayer.enableEntityBoundingBoxNode(true);
        }
    }
    
    else if ( _paletteType == 2)
    {
        if ( ImGuiLayer::imageButton("5_56mm.png", 50, 50) )
        {
            _selectedItem = static_cast<int>(EditorEntityType::ITEM_556MM);
            itemSelected = true;
        }
        
        ImGui::SameLine();
        if ( ImGuiLayer::imageButton("9mm.png", 50, 50) )
        {
            _selectedItem = static_cast<int>(EditorEntityType::ITEM_9MM);
            itemSelected = true;
        }
        
        ImGui::SameLine();
        if ( ImGuiLayer::imageButton("Shell.png", 50, 50) )
        {
            _selectedItem = static_cast<int>(EditorEntityType::ITEM_SHELL);
            itemSelected = true;
        }
        
        if ( ImGuiLayer::imageButton("Axe.png", 50, 50) )
        {
            _selectedItem = static_cast<int>(EditorEntityType::ITEM_AXE);
            itemSelected = true;
        }
        
        ImGui::SameLine();
        if ( ImGuiLayer::imageButton("Glock17.png", 50, 50) )
        {
            _selectedItem = static_cast<int>(EditorEntityType::ITEM_GLOCK17);
            itemSelected = true;
        }
        
        ImGui::SameLine();
        if ( ImGuiLayer::imageButton("M16A2.png", 50, 50) )
        {
            _selectedItem = static_cast<int>(EditorEntityType::ITEM_M16A2);
            itemSelected = true;
        }
        
        if ( ImGuiLayer::imageButton("M1897.png", 50, 50) )
        {
            _selectedItem = static_cast<int>(EditorEntityType::ITEM_M1897);
            itemSelected = true;
        }
        
        if ( itemSelected )
        {
            _gmxLayer.setCommand(_gmxLayer.getAddEntityToolCommand());
            _gmxLayer.enableEntityBoundingBoxNode(false);
        }
    }

    ImGui::PopStyleColor(3);
    ImGui::End();
}



