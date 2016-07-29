//
//  NavigatorLayer.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 28..
//
//

#pragma once

#include "TileBase.hpp"
#include "GMXlayer.hpp"
#include "SizeProtocol.h"
#include "TileHelperFunctions.hpp"
#include "StringHelper.hpp"

#include "imgui.h"
#include "imgui_internal.h"

namespace realtrick
{
    namespace editor
    {
        
        class GMXLayer;
        class EditorEntity;
        
        class NavigatorLayer : public cocos2d::Node
        {
            
        public:
            
            explicit NavigatorLayer(GMXLayer& gmxLayer) :
            _gmxLayer(gmxLayer),
            _layerSize(cocos2d::Size(200,200)),
            _layerPosition(cocos2d::Vec2(70, 100)),
            _boundingBoxPadding(cocos2d::Rect::ZERO),
            _centerViewParam(cocos2d::Vec2::ZERO)
            {}
            
            virtual ~NavigatorLayer()
            {
                _tileMarks.clear();
                _entityMarks.clear();
            }
            
            static NavigatorLayer* create(GMXLayer& gmxLayer)
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
            
            virtual bool init() override
            {
                if ( !cocos2d::Node::init() )
                    return false;
                
                _defaultTile = static_cast<TileType>(_gmxLayer.getFile().defaultTile);
                
                if ( _defaultTile == TileType::DIRT ) _defaultTileTextureName = cocos2d::Director::getInstance()->getTextureCache()->addImage("default_tile_dirt.png")->getName();
                else if ( _defaultTile == TileType::GRASS ) _defaultTileTextureName = cocos2d::Director::getInstance()->getTextureCache()->addImage("default_tile_grass.png")->getName();
                else if ( _defaultTile == TileType::WATER ) _defaultTileTextureName = cocos2d::Director::getInstance()->getTextureCache()->addImage("default_tile_water.png")->getName();
                else if ( _defaultTile == TileType::HILL ) _defaultTileTextureName = cocos2d::Director::getInstance()->getTextureCache()->addImage("default_tile_hill.png")->getName();
                
                return true;
                
            }
            
            void showLayer(bool& opened)
            {
                ImGuiContext& g = *GImGui;
                float height = g.FontBaseSize + g.Style.FramePadding.y * 2.0f;
                
                _layerSize = cocos2d::Size(cocos2d::Director::getInstance()->getVisibleSize().width * 0.15f,
                                           cocos2d::Director::getInstance()->getVisibleSize().width * 0.15f + height + g.Style.WindowPadding.y * 2);
                _layerSize.width = std::min(_layerSize.width, 300.0f);
                _layerSize.height = std::min(_layerSize.height, 300.0f);
                
                ImGui::SetNextWindowPos(ImVec2(_layerPosition.x, _layerPosition.y), ImGuiSetCond_Once);
                ImGui::SetNextWindowSize(ImVec2(_layerSize.width, _layerSize.height), ImGuiSetCond_Always);
                
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
                ImGui::Begin("navigator", &opened,
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
                
                static cocos2d::Vec2 mousePosInCocos2dMatrix;
                mousePosInCocos2dMatrix = cocos2d::Vec2(ImGui::GetIO().MousePos.x, ImGui::GetIO().DisplaySize.y - ImGui::GetIO().MousePos.y);
                
                if ( ImGui::IsMouseHoveringWindow() && ImGui::GetIO().MouseClicked[0] )
                {
                    cocos2d::Rect boundingBox(_layerPosition.x, ImGui::GetIO().DisplaySize.y - _layerSize.height - _layerPosition.y, _layerSize.width, _layerSize.height);
                    if ( boundingBox.containsPoint(mousePosInCocos2dMatrix) )
                    {
                        GMXLayer::enableTitleClicked();
                    }
                }
                
                ImVec2 size(_layerSize.width - g.Style.WindowPadding.x * 2.0f, _layerSize.height - height - g.Style.WindowPadding.y * 2.0f);
                ImGui::InvisibleButton("##dummy", size);
                
                ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y - size.y - ImGui::GetStyle().WindowPadding.y));
                
                ImGui::Image(reinterpret_cast<ImTextureID>(_defaultTileTextureName), ImVec2(size.x, size.y));
                
                ImDrawList* list = ImGui::GetWindowDrawList();
                ImGui::SetCursorScreenPos(ImVec2(_boundingBoxPadding.origin.x, _boundingBoxPadding.origin.y));
                ImU32 col = ImColor(ImVec4(1, 1, 1, 1));
                
                cocos2d::Size gmxCanvasSize = _gmxLayer.getLayerSize();
                cocos2d::Vec2 canvasOrigin = cocos2d::Vec2(_boundingBoxPadding.origin.x, _boundingBoxPadding.origin.y + _boundingBoxPadding.size.height);
                
                cocos2d::Size selectRegionSize = cocos2d::Size(gmxCanvasSize.width * _boundingBoxPadding.size.width / _gmxLayer.getWorldSize().width,
                                                               gmxCanvasSize.height * _boundingBoxPadding.size.height / _gmxLayer.getWorldSize().height);
                
                cocos2d::Rect movableRect = cocos2d::Rect(canvasOrigin.x + selectRegionSize.width / 2,
                                                          canvasOrigin.y - selectRegionSize.height / 2,
                                                          _boundingBoxPadding.size.width - selectRegionSize.width,
                                                          _boundingBoxPadding.size.height - selectRegionSize.height);
                
                
                ImGui::PushClipRect(ImVec2(_boundingBoxPadding.origin.x,
                                           _boundingBoxPadding.origin.y),
                                    ImVec2(_boundingBoxPadding.origin.x + _boundingBoxPadding.size.width,
                                           _boundingBoxPadding.origin.y + _boundingBoxPadding.size.height - 2),
                                    false);
                
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
                
                for(auto &ent : _entityMarks)
                {
                    auto entity = ent.second;
                    std::string type = "player" + _to_string(static_cast<int>(entity->getPlayerType())) + ".png";
                    auto texID = reinterpret_cast<ImTextureID>(cocos2d::Director::getInstance()->getTextureCache()->addImage(type)->getName());
                    
                    ImVec2 param = ImVec2(entity->getPosition().x / worldSize.x, entity->getPosition().y / worldSize.y);
                    ImVec2 origin = ImVec2(canvasOrigin.x + param.x * _boundingBoxPadding.size.width, canvasOrigin.y - param.y * _boundingBoxPadding.size.height);
                    
                    list->AddImage(texID, origin, ImVec2(origin.x + 2, origin.y - 2));
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
                    cocos2d::Rect boundingBox(_layerPosition.x, ImGui::GetIO().DisplaySize.y - _layerPosition.y - height, _layerSize.width, height);
                    if ( boundingBox.containsPoint(cocos2d::Vec2(ImGui::GetIO().MousePos.x, ImGui::GetIO().DisplaySize.y - ImGui::GetIO().MousePos.y)) )
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
                    cocos2d::Rect boundingBox(canvasOrigin.x, ImGui::GetIO().DisplaySize.y - canvasOrigin.y, _boundingBoxPadding.size.width, _boundingBoxPadding.size.height);
                    if ( boundingBox.containsPoint(cocos2d::Vec2(ImGui::GetIO().MousePos.x, ImGui::GetIO().DisplaySize.y - ImGui::GetIO().MousePos.y)) )
                    {
                        ImVec2 canvasPos = ImGui::GetCursorScreenPos();
                        cocos2d::Vec2 mousePosInCanvas = cocos2d::Vec2(ImGui::GetIO().MousePos.x - canvasPos.x, _boundingBoxPadding.size.height - (ImGui::GetIO().MousePos.y - canvasPos.y));
                        mousePosInCanvas.clamp(cocos2d::Vec2(selectRegionSize / 2), cocos2d::Vec2(_boundingBoxPadding.size - selectRegionSize / 2));
                        
                        _centerViewParam = cocos2d::Vec2((mousePosInCanvas.x - selectRegionSize.width / 2) / (_boundingBoxPadding.size.width - selectRegionSize.width),
                                                         (mousePosInCanvas.y - selectRegionSize.height / 2) / (_boundingBoxPadding.size.height - selectRegionSize.height));
                        _gmxLayer.setCenterViewParameter(_centerViewParam);
                    }
                }
                
                ImGui::End();
                ImGui::PopStyleVar();
            }
            
            void setTile(int x, int y, const TileBase& tile)
            {
                int numOfTileX = _gmxLayer.getFile().numOfTileX;
                int key = indexToNumber(x, y, numOfTileX, DUMMY_TILE_SIZE);
                if ( tile.getTileType() == _defaultTile )
                {
                    _tileMarks.erase(key);
                }
                else
                {
                    if ( tile.getTileType() == TileType::DIRT)
                    {
                        _tileMarks[key] = reinterpret_cast<ImTextureID>(cocos2d::Director::getInstance()->getTextureCache()->addImage("dirt_mark.png")->getName());
                    }
                    else if ( tile.getTileType() == TileType::GRASS )
                    {
                        _tileMarks[key] = reinterpret_cast<ImTextureID>(cocos2d::Director::getInstance()->getTextureCache()->addImage("grass_mark.png")->getName());
                    }
                    else if ( tile.getTileType() == TileType::WATER )
                    {
                        _tileMarks[key] = reinterpret_cast<ImTextureID>(cocos2d::Director::getInstance()->getTextureCache()->addImage("water_mark.png")->getName());
                    }
                    else if ( tile.getTileType() == TileType::HILL )
                    {
                        _tileMarks[key] = reinterpret_cast<ImTextureID>(cocos2d::Director::getInstance()->getTextureCache()->addImage("hill_mark.png")->getName());
                    }
                }
            }
            
            void addEntity(EditorEntity* ent) { _entityMarks[ent->getID()] = ent; }
            void eraseEntity(int id) { _entityMarks.erase(id); }
            
        private:
            
            GMXLayer& _gmxLayer;
            
            cocos2d::Size _layerSize;
            cocos2d::Vec2 _layerPosition;
            cocos2d::Rect _boundingBoxPadding;
            
            cocos2d::Vec2 _centerViewParam;
            
            std::map<int, ImTextureID> _tileMarks;
            std::map<int, EditorEntity*> _entityMarks;
            
            TileType _defaultTile;
            GLint _defaultTileTextureName;
            
        };
        
    }
}









