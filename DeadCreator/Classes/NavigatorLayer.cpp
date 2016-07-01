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
#include "SizeProtocol.h"
using namespace cocos2d;

NavigatorLayer::NavigatorLayer(EditScene2& imguiLayer, GMXLayer2& gmxLayer) :
_imguiLayer(imguiLayer),
_gmxLayer(gmxLayer),
_layerSize(Size(200,200)),
_layerPosition(Vec2(50, 50)),
_centerViewParam(Vec2::ZERO),
_centerViewPosition(Vec2(_layerSize / 2))
{
}


NavigatorLayer::~NavigatorLayer()
{
}


NavigatorLayer* NavigatorLayer::create(EditScene2& imguiLayer, GMXLayer2& gmxLayer)
{
    auto ret = new (std::nothrow) NavigatorLayer(imguiLayer, gmxLayer);
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
    
    auto file = _gmxLayer.getFile();
    _marks.resize(file.numOfTileX * file.numOfTileY);
    
    return true;
}


void NavigatorLayer::showLayer(bool* opened)
{
    ImGuiState& g = *GImGui;
    float height = g.FontBaseSize + g.Style.FramePadding.y * 2.0f;
    
    if ( _layerPosition.x < SizeProtocol::WINDOW_PADDING )
    {
        _layerPosition.x = SizeProtocol::WINDOW_PADDING;
    }
    
    if ( _layerPosition.y < height + SizeProtocol::WINDOW_PADDING )
    {
        _layerPosition.y = height + SizeProtocol::WINDOW_PADDING;
    }
    
    if ( _layerPosition.x + _layerSize.width > g.IO.DisplaySize.x - SizeProtocol::WINDOW_PADDING )
    {
        _layerPosition.x = g.IO.DisplaySize.x - _layerSize.width - SizeProtocol::WINDOW_PADDING;
    }
    
    if ( _layerPosition.y + _layerSize.height > g.IO.DisplaySize.y - SizeProtocol::WINDOW_PADDING - SizeProtocol::STATUSBAR_HEIGHT )
    {
        _layerPosition.y = g.IO.DisplaySize.y - _layerSize.height - SizeProtocol::WINDOW_PADDING - SizeProtocol::STATUSBAR_HEIGHT;
    }
    
    _layerSize = Size(Director::getInstance()->getVisibleSize().width * 0.15f, Director::getInstance()->getVisibleSize().width * 0.15f + height + g.Style.WindowPadding.y * 2);
    _layerSize.width = std::min(_layerSize.width, 300.0f);
    _layerSize.height = std::min(_layerSize.height, 300.0f);
    
    ImGui::SetNextWindowPos(ImVec2(_layerPosition.x, _layerPosition.y), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(_layerSize.width, _layerSize.height), ImGuiSetCond_Always);
    
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.8200000, 0.8200000, 0.8200000, 1.0000000));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
    ImGui::Begin("navigator", opened,
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
    
    ImVec2 size(_layerSize.width - g.Style.WindowPadding.x * 2.0f, _layerSize.height - height - g.Style.WindowPadding.y * 2.0f);
    ImGui::InvisibleButton("##dummy", size);
    
    ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y - size.y - ImGui::GetStyle().WindowPadding.y));
    _imguiLayer.image("default_tile_dirt.png", size.x, size.y);
    
    ImDrawList* list = ImGui::GetWindowDrawList();
    ImGui::SetCursorScreenPos(ImVec2(_boundingBoxPadding.origin.x, _boundingBoxPadding.origin.y));
    ImU32 col = ImColor(ImVec4(1, 1, 1, 1));
    
    Size gmxCanvasSize = _gmxLayer.getLayerSize();
    Vec2 canvasOrigin = Vec2(_boundingBoxPadding.origin.x, _boundingBoxPadding.origin.y + _boundingBoxPadding.size.height);
    
    Size selectRegionSize = Size(gmxCanvasSize.width * _boundingBoxPadding.size.width / _gmxLayer.getWorldSize().width,
                                 gmxCanvasSize.height * _boundingBoxPadding.size.height / _gmxLayer.getWorldSize().height);
    
    Rect movableRect = Rect(canvasOrigin.x + selectRegionSize.width / 2,
                            canvasOrigin.y - selectRegionSize.height / 2,
                            _boundingBoxPadding.size.width - selectRegionSize.width,
                            _boundingBoxPadding.size.height - selectRegionSize.height);
    
    for(auto &mark : _marks)
    {
        list->AddImage(mark.texture, mark.origin, ImVec2(mark.origin.x + mark.size.x, mark.origin.y + mark.size.y));
    }
    
    _centerViewParam = _gmxLayer.getCenterViewParameter();
    list->AddRect(ImVec2(movableRect.origin.x + (movableRect.size.width * _centerViewParam.x) - selectRegionSize.width / 2,
                         movableRect.origin.y - (movableRect.size.height * _centerViewParam.y) - selectRegionSize.height / 2),
                  ImVec2(movableRect.origin.x + (movableRect.size.width * _centerViewParam.x) + selectRegionSize.width / 2,
                         movableRect.origin.y - (movableRect.size.height * _centerViewParam.y) + selectRegionSize.height / 2),
                  col , 5.0f);

    
    static bool titleClicked = false;
    if ( ImGui::GetIO().MouseClicked[0] )
    {
        Rect boundingBox(_layerPosition.x, ImGui::GetIO().DisplaySize.y - _layerPosition.y - height, _layerSize.width, height);
        if ( boundingBox.containsPoint(Vec2(ImGui::GetIO().MousePos.x, ImGui::GetIO().DisplaySize.y - ImGui::GetIO().MousePos.y)) )
        {
            titleClicked = true;
        }
    }
    else if ( ImGui::GetIO().MouseReleased[0] )
    {
        titleClicked = false;
    }
    
    if ( (ImGui::IsMouseDragging() || ImGui::GetIO().MouseClicked[0]) && !titleClicked && ImGui::IsMouseHoveringWindow() )
    {
        Rect boundingBox(canvasOrigin.x, ImGui::GetIO().DisplaySize.y - canvasOrigin.y, _boundingBoxPadding.size.width, _boundingBoxPadding.size.height);
        if ( boundingBox.containsPoint(Vec2(ImGui::GetIO().MousePos.x, ImGui::GetIO().DisplaySize.y - ImGui::GetIO().MousePos.y)) )
        {
            ImVec2 canvasPos = ImGui::GetCursorScreenPos();
            _mousePosInCanvas = Vec2(ImGui::GetIO().MousePos.x - canvasPos.x, _boundingBoxPadding.size.height - (ImGui::GetIO().MousePos.y - canvasPos.y));
            _mousePosInCanvas.clamp(Vec2(selectRegionSize / 2), Vec2(_boundingBoxPadding.size - selectRegionSize / 2));
            
            _centerViewParam = Vec2((_mousePosInCanvas.x - selectRegionSize.width / 2) / (_boundingBoxPadding.size.width - selectRegionSize.width),
                                    (_mousePosInCanvas.y - selectRegionSize.height / 2) / (_boundingBoxPadding.size.height - selectRegionSize.height));
            _gmxLayer.setCenterViewParameter(_centerViewParam);
        }
    }
    
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}


void NavigatorLayer::setTile(int x, int y, TileType type)
{
    Vec2 origin(_boundingBoxPadding.origin.x, _boundingBoxPadding.origin.y + _boundingBoxPadding.size.height);
//    MarkInfo
//    _marks.push_back(MarkInfo(ImVec2(origin.x + px * _boundingBoxPadding.size.width, origin.y - py * _boundingBoxPadding.size.height), ImVec4(0,1,0,1)));
}








