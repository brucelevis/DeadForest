//
//  PlayGameLayer.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 28..
//
//


#include "DummyScene.hpp"
#include "EditScene.hpp"
using namespace realtrick::editor;

#include "Game.hpp"
#include "GameResource.hpp"
#include "RenderingSystem.hpp"
using namespace realtrick::client;
using namespace cocos2d;


void PlayGameLayer::showLayer(bool& opened)
{
    static bool isStatusOn = true;
    static bool isPlayerInfo = true;
    static bool isNavGraphOn = true;
    static bool isCellSpaceOn = true;
    static bool isPhysicsViewOn = true;
    static bool isLocationViewOn = true;
    
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2  - GAME_SCREEN_WIDTH / 2,
                                   ImGui::GetIO().DisplaySize.y / 2  - GAME_SCREEN_HEIGHT / 2), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT + 25), ImGuiSetCond_Once);
    
    ImGui::Begin("simulator", &opened, ImVec2(0,0), 0.0f,
                 ImGuiWindowFlags_ShowBorders |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoBringToFrontOnFocus);
    
    setPosition(ImGui::GetWindowPos().x, ImGui::GetIO().DisplaySize.y - ImGui::GetWindowPos().y - GAME_SCREEN_HEIGHT - 25);
    
    auto origin = Vec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y);
    auto canvasSize = cocos2d::Size(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
    ImGui::InvisibleButton("##dummy", ImVec2(canvasSize.width, canvasSize.height));
    
    // game layer
    if ( _isGameStarted )
    {
        ImGui::SetCursorScreenPos(ImVec2(origin.x, origin.y));
        
        auto game = _gameLayer->getGame();
        auto drawList = ImGui::GetWindowDrawList();
        
        if ( isStatusOn )
        {
            static auto renderer = Director::getInstance()->getRenderer();
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.00, 1.00, 1.00, 1.00));
            ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
            ImGui::Text("Vertices: %d", static_cast<int>(renderer->getDrawnVertices()));
            ImGui::Text("Batches: %d", static_cast<int>(renderer->getDrawnBatches()));
            ImGui::PopStyleColor();
        }
        
        if ( isPlayerInfo )
        {
            ImVec2 before = ImGui::GetCursorScreenPos();
            ImGui::SetCursorScreenPos(ImVec2(origin.x, origin.y + GAME_SCREEN_HEIGHT - 80));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.00, 1.00, 1.00, 1.00));
            
            auto player = game->getPlayerPtr();
            ImGui::Text("x: %.0f, y: %.0f", player->getWorldPosition().x, player->getWorldPosition().y);
            ImGui::Text("hp: %d", player->getBlood());
            ImGui::Text("state: %s", player->getStateName().c_str());
            
            ImGui::PopStyleColor();
            ImGui::SetCursorScreenPos(before);
        }
        
        if ( isNavGraphOn )
        {
            auto resource = game->getGameResource();
            
            // rectangle lines
            for(int i = 0 ; i < resource->getWorldHeight() ; i = i + resource->getTileHeight() / 4 )
            {
                auto a = worldToLocal(origin, Vec2(0, i));
                auto b = worldToLocal(origin, Vec2(resource->getWorldWidth(), i));
                drawList->AddLine(ImVec2(a.x, a.y), ImVec2(b.x, b.y), ImColor(ImVec4(1.0, 1.0, 1.0, 0.1)));
            }
            
            for(int i = 0 ; i < resource->getWorldWidth() ; i = i + resource->getTileWidth() / 4 )
            {
                auto a = worldToLocal(origin, Vec2(i, 0));
                auto b = worldToLocal(origin, Vec2(i, resource->getWorldHeight()));
                drawList->AddLine(ImVec2(a.x, a.y), ImVec2(b.x, b.y), ImColor(ImVec4(1.0, 1.0, 1.0, 0.1)));
            }
            
            // center recet
            auto indices = getFocusedTileIndex(game->getPlayerPtr()->getWorldPosition(), 128, 128, 4);
            const auto& tiles = resource->getTileData();
            for( int i = indices.second - Prm.getValueAsInt("numOfViewableTileY") / 2 ;
                i < indices.second + Prm.getValueAsInt("numOfViewableTileY") / 2 ; ++ i)
            {
                for(int j = indices.first - Prm.getValueAsInt("numOfViewableTileX") / 2 ;
                    j < indices.first + Prm.getValueAsInt("numOfViewableTileX") / 2 ; ++ j)
                {
                    if ( j < 0 || i < 0 || i >= resource->getNumOfTileY() || j >= resource->getNumOfTileX() ) continue;
                    
                    auto left = worldToLocal(origin, tiles[i][j].getPosition() + Vec2(-64, 0));
                    auto bottom = worldToLocal(origin, tiles[i][j].getPosition() + Vec2(0, -64));
                    auto right = worldToLocal(origin, tiles[i][j].getPosition() + Vec2(64, 0));
                    
                    drawList->AddLine(ImVec2(right.x, right.y), ImVec2(bottom.x, bottom.y), ImColor(ImVec4(1.0, 1.0, 1.0, 0.2)));
                    drawList->AddLine(ImVec2(bottom.x, bottom.y), ImVec2(left.x, left.y), ImColor(ImVec4(1.0, 1.0, 1.0, 0.2)));
                }
            }
        }
        
        if ( isCellSpaceOn )
        {
            auto cellSpace = game->getCellSpace();
            
            auto currCellIndex = cellSpace->positionToIndex(game->getPlayerPtr()->getWorldPosition());
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.00, 1.00, 0.00, 1.00));
            ImGui::Text(std::string("current cell: " + _to_string(currCellIndex)).c_str(), NULL);
            ImGui::PopStyleColor();

            const auto& cells = cellSpace->getCells();
            for (const auto& cell : cells )
            {
                const auto& cellSize = cell.boundingBox.size;
                
                const auto cellOrigin = worldToLocal(origin, cell.boundingBox.origin);
                const auto top = worldToLocal(origin, cell.boundingBox.origin + Vec2(0, cellSize.height));
                const auto right = worldToLocal(origin, cell.boundingBox.origin + Vec2(cellSize.width , 0));
                
                drawList->AddLine(ImVec2(cellOrigin.x, cellOrigin.y), ImVec2(top.x, top.y), ImColor(ImVec4(1.0, 0.0, 0.0, 0.5)));
                drawList->AddLine(ImVec2(cellOrigin.x, cellOrigin.y), ImVec2(right.x, right.y), ImColor(ImVec4(1.0, 0.0, 0.0, 0.5)));
            }
        }
        
        if ( isPhysicsViewOn )
        {
            auto cellSpace = game->getCellSpace();
            auto cellIndices = cellSpace->getNeighborCells(game->getPlayerPtr()->getWorldPosition());
            for ( const auto& idx : cellIndices )
            {
                const Cell& currCell = cellSpace->getCell(idx);
                for ( const auto& ent : currCell.members )
                {
                    if (ent->getEntityType() == EntityType::ENTITY_FINITE ||
                        isMasked(ent->getFamilyMask(), FamilyMask::BULLET_BASE) )
                        continue;
                    
                    auto rad = ent->getBoundingRadius() * game->getRenderingSysetm()->getZoomScale().x;
                    auto center = worldToLocal(origin, ent->getWorldPosition());
                    drawList->AddCircle(ImVec2(center.x, center.y), rad, ImColor(ImVec4(1.0, 0.0, 1.0, 0.7)), 20, 0.0f);
                }
            }
            
            auto walls = game->getNeighborWalls(game->getPlayerPtr()->getWorldPosition(), 0.0f);
            for( const auto& wall : walls )
            {
                for( int i = 0 ; i < wall.vertices.size() - 1; ++ i)
                {
                    auto a = worldToLocal(origin, wall.vertices[i]);
                    auto b = worldToLocal(origin, wall.vertices[i + 1]);
                    drawList->AddLine(ImVec2(a.x, a.y), ImVec2(b.x, b.y), ImColor(ImVec4(1.0, 0.0, 0.0, 0.5)));
                }
                auto a = worldToLocal(origin, wall.vertices.back());
                auto b = worldToLocal(origin, wall.vertices.front());
                drawList->AddLine(ImVec2(a.x, a.y), ImVec2(b.x, b.y), ImColor(ImVec4(1.0, 0.0, 0.0, 0.5)));
            }
            
        }
        
        if ( isLocationViewOn )
        {
            const auto& locations = game->getGameResource()->getLocations();
            for( const auto& location : locations )
            {
                const auto& name = location.first;
                const auto& rect = location.second;
                
                Vec2 rectOrigin = worldToLocal(origin, rect.origin);
                Vec2 rectDest = worldToLocal(origin, rect.origin + rect.size);
                
                drawList->AddRect(ImVec2(rectOrigin.x, rectOrigin.y),
                                  ImVec2(rectDest.x, rectDest.y),
                                  ImColor(ImVec4(1.0, 1.0, 1.0, 0.6)));
                drawList->AddRectFilled(ImVec2(rectOrigin.x, rectOrigin.y),
                                        ImVec2(rectDest.x, rectDest.y),
                                        ImColor(ImVec4(0.2, 0.2, 0.8, 0.2)));
                
                drawList->AddText(ImVec2(rectOrigin.x, rectOrigin.y), ImColor(ImVec4(1.0, 1.0, 0.0, 1.0)), name.c_str());
            }
        }
        
    }
    ImGui::End();
    
    if ( !opened ) closeLayer();
    
    // setting layer
    if ( _isGameStarted )
    {
        ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiSetCond_Once);
        ImGui::Begin("setting", NULL, ImGuiWindowFlags_ShowBorders);
        
        if (ImGui::TreeNode("debug"))
        {
            ImGui::Checkbox("status", &isStatusOn);
            ImGui::Checkbox("player info", &isPlayerInfo);
            ImGui::Checkbox("graph", &isNavGraphOn);
            ImGui::Checkbox("cell", &isCellSpaceOn);
            ImGui::Checkbox("physics", &isPhysicsViewOn);
            ImGui::Checkbox("location", &isLocationViewOn);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("property"))
        {
            ImGui::Text("can not use yet.");
            ImGui::TreePop();
        }
        
        ImGui::End();
    }
    
    // logger
    if ( _isGameStarted )
    {
        auto game = _gameLayer->getGame();
        
        ImGui::SetNextWindowSize(ImVec2(1000, 150), ImGuiSetCond_Once);
        ImGui::SetNextWindowPos(ImVec2(100, ImGui::GetIO().DisplaySize.y - 200), ImGuiSetCond_Once);
        ImGui::Begin("Console Log", NULL, ImGuiWindowFlags_ShowBorders);
        
        if (ImGui::Button("Clear")) game->clearLogs();
        ImGui::SameLine(); bool copy = ImGui::Button("Copy");
        ImGui::Separator();
        
        ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
        
        if (copy)  ImGui::LogToClipboard();
        ImGui::TextUnformatted(game->getLogString().c_str());
        if ( game->isLogAdded() ) ImGui::SetScrollHere(1.0f);
        game->isLogAdded() = false;
        
        ImGui::EndChild();
        ImGui::End();
    }
}

void PlayGameLayer::playGame()
{
    _gameLayer = realtrick::client::DummyScene::create(this);
    addChild(_gameLayer);
}

void PlayGameLayer::closeLayer()
{
    _gameLayer->removeFromParentAndCleanup(true);
    _imguiLayer->stopGame();
}


cocos2d::Vec2 PlayGameLayer::worldToLocal(const cocos2d::Vec2& p)
{
    auto game = _gameLayer->getGame();
    _cameraTransform.identity();
    _cameraTransform.translate(-game->getPlayerPtr()->getWorldPosition());
    auto transformedVector = _cameraTransform.getTransformedVector(p);
    return cocos2d::Vec2(transformedVector.x * game->getRenderingSysetm()->getZoomScale().x,
                         transformedVector.y * game->getRenderingSysetm()->getZoomScale().y);
}


cocos2d::Vec2 PlayGameLayer::worldToLocal(const cocos2d::Vec2& origin, const cocos2d::Vec2& p)
{
    auto local = worldToLocal(p);
    return cocos2d::Vec2(origin.x + local.x + GAME_SCREEN_WIDTH / 2, origin.y - local.y + GAME_SCREEN_HEIGHT / 2);
}







