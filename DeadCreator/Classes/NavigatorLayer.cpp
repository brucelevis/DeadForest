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
                 ImGuiWindowFlags_NoScrollWithMouse |
                 ImGuiWindowFlags_ShowBorders);
    
    _layerPosition.setPoint(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
    _layerSize.setSize(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
    
    _boundingBoxPadding.setRect(_layerPosition.x + g.Style.WindowPadding.x,
                                _layerPosition.y + g.Style.WindowPadding.y + height,
                                _layerSize.width - g.Style.WindowPadding.x * 2.0f,
                                _layerSize.height - g.Style.WindowPadding.y * 2.0f - height);
    
//    static ImVec2 size(100, 100), offset(50, 20);
//    ImGui::TextWrapped("On a per-widget basis we are occasionally clipping text CPU-side if it won't fit in its frame. Otherwise we are doing coarser clipping + passing a scissor rectangle to the renderer. The system is designed to try minimizing both execution and CPU/GPU rendering cost.");
//    ImGui::DragFloat2("size", (float*)&size, 0.5f, 0.0f, 200.0f, "%.0f");
//    ImGui::TextWrapped("(Click and drag)");
//    ImVec2 pos = ImGui::GetCursorScreenPos();
//    ImVec4 clip_rect(pos.x, pos.y, pos.x+size.x, pos.y+size.y);
//    ImGui::InvisibleButton("##dummy", size);
//    if (ImGui::IsItemActive() && ImGui::IsMouseDragging()) { offset.x += ImGui::GetIO().MouseDelta.x; offset.y += ImGui::GetIO().MouseDelta.y; }
    
    static ImVec2 size(_layerSize.width - g.Style.WindowPadding.x * 2.0f, _layerSize.height - height - g.Style.WindowPadding.y * 2.0f);
    ImGui::InvisibleButton("##dummy", size);
    
    ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y - size.y - ImGui::GetStyle().WindowPadding.y));
    _imguiLayer.image("default_tile_dirt.png", size.x, size.y);
    
    ImDrawList* list = ImGui::GetWindowDrawList();
    ImGui::SetCursorScreenPos(ImVec2(_boundingBoxPadding.origin.x, _boundingBoxPadding.origin.y));
    ImU32 col = ImColor(ImVec4(1, 1, 1, 1));
    
    Size canvasSize = Size(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
    Size gmxCanvasSize = _gmxLayer.getLayerSize();
    Vec2 canvasOrigin = Vec2(_boundingBoxPadding.origin.x, _boundingBoxPadding.origin.y + canvasSize.height);
    
    Size selectRegionSize = Size(gmxCanvasSize.width * canvasSize.width / _gmxLayer.getWorldSize().width, gmxCanvasSize.height * canvasSize.height / _gmxLayer.getWorldSize().height);
    Rect movableRect = Rect(canvasOrigin.x + selectRegionSize.width / 2,
                            canvasOrigin.y - selectRegionSize.height / 2,
                            canvasSize.width - selectRegionSize.width,
                            canvasSize.height - selectRegionSize.height);
    
    _centerViewParam = _gmxLayer.getCenterViewParameter();
    list->AddRect(ImVec2(movableRect.origin.x + (movableRect.size.width * _centerViewParam.x) - selectRegionSize.width / 2,
                         movableRect.origin.y - (movableRect.size.height * _centerViewParam.y) - selectRegionSize.height / 2),
                  ImVec2(movableRect.origin.x + (movableRect.size.width * _centerViewParam.x) + selectRegionSize.width / 2,
                         movableRect.origin.y - (movableRect.size.height * _centerViewParam.y) + selectRegionSize.height / 2),
                  col , 5.0f);
    
    if ( ImGui::GetIO().MouseClicked[0] )
    {
        Rect boundingBox(canvasOrigin.x, ImGui::GetIO().DisplaySize.y - canvasOrigin.y, canvasSize.width, canvasSize.height);
        if ( boundingBox.containsPoint(Vec2(ImGui::GetIO().MousePos.x, ImGui::GetIO().DisplaySize.y - ImGui::GetIO().MousePos.y)) )
        {
            ImVec2 canvasPos = ImGui::GetCursorScreenPos();
            _mousePosInCanvas = Vec2(ImGui::GetIO().MousePos.x - canvasPos.x, canvasSize.height - (ImGui::GetIO().MousePos.y - canvasPos.y));
            _mousePosInCanvas.clamp(Vec2(selectRegionSize / 2), Vec2(canvasSize - selectRegionSize / 2));
            
            _centerViewParam = Vec2((_mousePosInCanvas.x - selectRegionSize.width / 2) / (canvasSize.width - selectRegionSize.width),
                                    (_mousePosInCanvas.y - selectRegionSize.height / 2) / (canvasSize.height - selectRegionSize.height));
            _gmxLayer.setCenterViewParameter(_centerViewParam);
        }
    }
    
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}









