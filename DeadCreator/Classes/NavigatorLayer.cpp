//
//  NavigatorLayer.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 28..
//
//

#include "NavigatorLayer.hpp"
#include "EditScene2.hpp"
#include "GMXLayer2.hpp"
using namespace cocos2d;

NavigatorLayer::NavigatorLayer(EditScene2& imguiLayer) :
_imguiLayer(imguiLayer),
_layerSize(Size(200,200)),
_layerPosition(Vec2(50, 50)),
_centerViewParam(Vec2::ZERO),
_centerViewPosition(Vec2(_layerSize / 2))
{
}


NavigatorLayer::~NavigatorLayer()
{
}


NavigatorLayer* NavigatorLayer::create(EditScene2& imguiLayer)
{
    auto ret = new (std::nothrow) NavigatorLayer(imguiLayer);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool NavigatorLayer::init()
{
    if ( !Node::init() )
        return false;
    
    return true;
}


void NavigatorLayer::showLayer(bool* opened)
{
    ImGuiState& g = *GImGui;
    float height = g.FontBaseSize + g.Style.FramePadding.y * 2.0f;
    
    _layerSize = Size(Director::getInstance()->getVisibleSize().width * 0.15f, Director::getInstance()->getVisibleSize().width * 0.15f + height + g.Style.WindowPadding.y * 2);
    
    ImGui::SetNextWindowPos(ImVec2(_layerPosition.x, _layerPosition.y), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(_layerSize.width, _layerSize.height), ImGuiSetCond_Always);
    
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.8200000, 0.8200000, 0.8200000, 1.0000000));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
    ImGui::Begin("navigator", opened,
                 ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_ShowBorders);
    
    _layerPosition.setPoint(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
    _layerSize.setSize(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
    
    _boundingBox.setRect(_layerPosition.x, _layerPosition.y + _layerSize.height, _layerSize.width, _layerSize.height);
    _boundingBoxPadding.setRect(_layerPosition.x + g.Style.WindowPadding.x,
                                _layerPosition.y + g.Style.WindowPadding.y + height,
                                _layerSize.width - g.Style.WindowPadding.x * 2.0f,
                                _layerSize.height - g.Style.WindowPadding.y * 2.0f - height);
    
    _imguiLayer.image("default_tile_dirt.png", _layerSize.width - g.Style.WindowPadding.x * 2.0f, _layerSize.height - height - g.Style.WindowPadding.y * 2.0f);
    
    ImDrawList* list = ImGui::GetWindowDrawList();
    ImGui::SetCursorScreenPos(ImVec2(_boundingBoxPadding.origin.x, _boundingBoxPadding.origin.y));
    ImU32 col = ImColor(ImVec4(1, 0, 0, 1));
    list->AddRect(ImVec2(_boundingBoxPadding.origin.x, _boundingBoxPadding.origin.y),
                  ImVec2(_boundingBoxPadding.origin.x + _boundingBoxPadding.size.width, _boundingBoxPadding.origin.y + _boundingBoxPadding.size.height),
                  col);
    
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}





