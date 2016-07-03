//
//  HistoryLayer.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 3..
//
//

#include "HistoryLayer.hpp"
#include "GMXLayer2.hpp"
using namespace cocos2d;

HistoryLayer::HistoryLayer(GMXLayer2& gmxLayer) :
_gmxLayer(gmxLayer),
_layerSize(Size(Director::getInstance()->getVisibleSize().width * 0.20f, Director::getInstance()->getVisibleSize().height * 0.5f)),
_layerPosition(Vec2(120, 70)),
_boundingBoxPadding(Rect::ZERO)
{
}


HistoryLayer::~HistoryLayer()
{
}


HistoryLayer* HistoryLayer::create(GMXLayer2& gmxLayer)
{
    auto ret = new (std::nothrow) HistoryLayer(gmxLayer);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool HistoryLayer::init()
{
    if ( !Node::init() )
        return false;
    
    return true;
}


void HistoryLayer::showLayer(bool* opened)
{
    ImGuiState& g = *GImGui;
    float height = g.FontBaseSize + g.Style.FramePadding.y * 2.0f;
    
    if ( _layerPosition.x < WINDOW_PADDING )
    {
        _layerPosition.x = WINDOW_PADDING;
    }
    
    if ( _layerPosition.y < height + WINDOW_PADDING )
    {
        _layerPosition.y = height + WINDOW_PADDING;
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
    ImGui::SetNextWindowSize(ImVec2(_layerSize.width, _layerSize.height), ImGuiSetCond_Once);
    
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.8200000, 0.8200000, 0.8200000, 1.0000000));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
    ImGui::Begin("history", opened,
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
    
    
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}









