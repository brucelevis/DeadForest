//
//  NavigatorLayer.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 28..
//
//

#include "NavigatorLayer.hpp"
#include "GMXLayer2.hpp"
#include "SizeProtocol.h"
#include "TileHelperFunctions.hpp"
using namespace cocos2d;

NavigatorLayer::NavigatorLayer(GMXLayer2& gmxLayer) :
_gmxLayer(gmxLayer),
_layerSize(Size(200,200)),
_layerPosition(Vec2(50, 50)),
_boundingBoxPadding(Rect::ZERO),
_centerViewParam(Vec2::ZERO)
{
}


NavigatorLayer::~NavigatorLayer()
{
}


NavigatorLayer* NavigatorLayer::create(GMXLayer2& gmxLayer)
{
    auto ret = new (std::nothrow) NavigatorLayer(gmxLayer);
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
    auto texName = cocos2d::Director::getInstance()->getTextureCache()->addImage("default_tile_dirt.png")->getName();
    ImGui::Image(reinterpret_cast<ImTextureID>(texName), ImVec2(size.x, size.y));
    
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
    
    
    ImGui::PushClipRect(ImVec2(_boundingBoxPadding.origin.x,
                               _boundingBoxPadding.origin.y),
                        ImVec2(_boundingBoxPadding.origin.x + _boundingBoxPadding.size.width,
                               _boundingBoxPadding.origin.y + _boundingBoxPadding.size.height - 2));
    
    auto file = _gmxLayer.getFile();
    ImVec2 worldSize = ImVec2(file.worldSize.width, file.worldSize.height);
    for(auto &tile : _tileMarks)
    {
        int key = tile.first;
        auto indices = numberToIndex(key, file.numOfTileX, DUMMY_TILE_SIZE);
        auto position = indexToPosition(indices.first, indices.second, file.tileWidth, file.tileHeight, DUMMY_TILE_SIZE);
        ImVec2 param = ImVec2(position.x / worldSize.x, position.y / worldSize.y);
        ImVec2 origin = ImVec2(canvasOrigin.x + param.x * _boundingBoxPadding.size.width, canvasOrigin.y - param.y * _boundingBoxPadding.size.height);
        ImVec2 size = ImVec2(_boundingBoxPadding.size.width * file.tileWidth / worldSize.x, _boundingBoxPadding.size.height * file.tileHeight / worldSize.y);
        list->AddImage(tile.second, origin, ImVec2(origin.x + size.x, origin.y - size.y));
    }
    
    _centerViewParam = _gmxLayer.getCenterViewParameter();
    list->AddRect(ImVec2(movableRect.origin.x + (movableRect.size.width * _centerViewParam.x) - selectRegionSize.width / 2,
                         movableRect.origin.y - (movableRect.size.height * _centerViewParam.y) - selectRegionSize.height / 2),
                  ImVec2(movableRect.origin.x + (movableRect.size.width * _centerViewParam.x) + selectRegionSize.width / 2,
                         movableRect.origin.y - (movableRect.size.height * _centerViewParam.y) + selectRegionSize.height / 2),
                  col , 5.0f);
    
    ImGui::PopClipRect();
    
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
            Vec2 mousePosInCanvas = Vec2(ImGui::GetIO().MousePos.x - canvasPos.x, _boundingBoxPadding.size.height - (ImGui::GetIO().MousePos.y - canvasPos.y));
            mousePosInCanvas.clamp(Vec2(selectRegionSize / 2), Vec2(_boundingBoxPadding.size - selectRegionSize / 2));
            
            _centerViewParam = Vec2((mousePosInCanvas.x - selectRegionSize.width / 2) / (_boundingBoxPadding.size.width - selectRegionSize.width),
                                    (mousePosInCanvas.y - selectRegionSize.height / 2) / (_boundingBoxPadding.size.height - selectRegionSize.height));
            _gmxLayer.setCenterViewParameter(_centerViewParam);
        }
    }
    
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}


void NavigatorLayer::setTile(int x, int y, const TileBase& tile)
{
    int key = indexToNumber(x, y, _gmxLayer.getFile().numOfTileX, DUMMY_TILE_SIZE);
    
    if ( tile.getType() == TileType::DIRT)
    {
        _tileMarks[key] = reinterpret_cast<ImTextureID>(cocos2d::Director::getInstance()->getTextureCache()->addImage("dirt_mark.png")->getName());
    }
    else if ( tile.getType() == TileType::GRASS )
    {
        _tileMarks[key] = reinterpret_cast<ImTextureID>(cocos2d::Director::getInstance()->getTextureCache()->addImage("grass_mark.png")->getName());
    }
    else if ( tile.getType() == TileType::WATER )
    {
        _tileMarks[key] = reinterpret_cast<ImTextureID>(cocos2d::Director::getInstance()->getTextureCache()->addImage("water_mark.png")->getName());
    }
    else if ( tile.getType() == TileType::HILL )
    {
        _tileMarks[key] = reinterpret_cast<ImTextureID>(cocos2d::Director::getInstance()->getTextureCache()->addImage("hill_mark.png")->getName());
    }
}








