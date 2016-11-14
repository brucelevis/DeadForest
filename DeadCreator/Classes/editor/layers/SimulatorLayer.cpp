//
//  SimulatorLayer.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 28..
//
//

#include <iterator>

#include "DummyScene.hpp"
#include "EditScene.hpp"
#include "SimulatorLayer.hpp"
using namespace realtrick::editor;

#include "Game.hpp"
#include "PhysicsManager.hpp"
#include "Wall.hpp"
#include "Camera2D.hpp"
#include "GameResource.hpp"
#include "RenderingSystem.hpp"
#include "SparseGraph.hpp"
#include "PathPlanner.hpp"
#include "PathEdge.hpp"
#include "EntityManager.hpp"
#include "HumanBase.hpp"
#include "BrainBase.hpp"
#include "Goals.hpp"
#include "GoalBase.hpp"
#include "GoalCompositeBase.hpp"
using namespace realtrick;
using namespace realtrick::client;
using namespace cocos2d;


const ImVec4 SimulatorLayer::BLACK = ImVec4(0.0, 0.0, 0.0, 1.0);
const ImVec4 SimulatorLayer::GRAYLISH_GREEN = ImVec4(0.2, 0.5, 0.2, 1.0);
const ImVec4 SimulatorLayer::GRAYLISH_RED = ImVec4(0.5, 0.2, 0.2, 1.0);
const ImVec4 SimulatorLayer::GRAYLISH_BLUE = ImVec4(0.2, 0.2, 0.5, 1.0);


SimulatorLayer::SimulatorLayer(EditScene* layer) : _imguiLayer(layer)
{
    _tcpSession.connect("127.0.0.1", "4242");
}


void SimulatorLayer::showLayer(bool& opened)
{
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2 - GAME_SCREEN_WIDTH / 2,
                                   ImGui::GetIO().DisplaySize.y / 2 - GAME_SCREEN_HEIGHT / 2), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT + 25), ImGuiSetCond_Once);
    
    ImGui::Begin("simulator", &opened, ImVec2(0, 0), 0.0f,
                 ImGuiWindowFlags_ShowBorders |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoBringToFrontOnFocus);
    
    setPosition(ImGui::GetWindowPos().x, ImGui::GetIO().DisplaySize.y - ImGui::GetWindowPos().y - GAME_SCREEN_HEIGHT - 25);
    
    _debugOrigin = Vec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y);
    auto canvasSize = cocos2d::Size(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
    ImGui::InvisibleButton("##dummy", ImVec2(canvasSize.width, canvasSize.height));
    
    // game layer
    if (_isGameStarted)
    {
        auto game = _gameLayer->getGame();
        if (!game->isGameEnded())
        {
            ImGui::SetCursorScreenPos(ImVec2(_debugOrigin.x, _debugOrigin.y));
            auto drawList = ImGui::GetWindowDrawList();
            
            if (_isPlayerInfo)
            {
                ImVec2 before = ImGui::GetCursorScreenPos();
                ImGui::SetCursorScreenPos(ImVec2(_debugOrigin.x, _debugOrigin.y + GAME_SCREEN_HEIGHT - 80));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.00, 1.00, 1.00, 1.00));
                
                auto player = game->getPlayerPtr();
                ImGui::Text("x: %.0f, y: %.0f", player->getWorldPosition().x, player->getWorldPosition().y);
                ImGui::Text("hp: %d", player->getBlood());
                ImGui::Text("state: %s", player->getStateName().c_str());
                
                ImGui::PopStyleColor();
                ImGui::SetCursorScreenPos(before);
            }
            
            if (_isGridOn)
            {
                auto resource = game->getGameResource();
                
                // rectangle lines
                for (int i = 0; i < resource->getWorldHeight(); i = i + resource->getTileHeight() / 4)
                {
                    auto a = worldToLocal(_debugOrigin, Vec2(0, i));
                    auto b = worldToLocal(_debugOrigin, Vec2(resource->getWorldWidth(), i));
                    drawList->AddLine(ImVec2(a.x, a.y), ImVec2(b.x, b.y), ImColor(ImVec4(1.0, 1.0, 1.0, 0.1)));
                }
                
                for (int i = 0; i < resource->getWorldWidth(); i = i + resource->getTileWidth() / 4)
                {
                    auto a = worldToLocal(_debugOrigin, Vec2(i, 0));
                    auto b = worldToLocal(_debugOrigin, Vec2(i, resource->getWorldHeight()));
                    drawList->AddLine(ImVec2(a.x, a.y), ImVec2(b.x, b.y), ImColor(ImVec4(1.0, 1.0, 1.0, 0.1)));
                }
                
                // center rect
                auto indices = getFocusedTileIndex(game->getPlayerPtr()->getWorldPosition(), 128, 128, 4);
                const auto& tiles = resource->getTileData();
                for (int i = indices.second - Prm.getValueAsInt("numOfViewableTileY") / 2;
                     i < indices.second + Prm.getValueAsInt("numOfViewableTileY") / 2; ++i)
                {
                    for (int j = indices.first - Prm.getValueAsInt("numOfViewableTileX") / 2;
                         j < indices.first + Prm.getValueAsInt("numOfViewableTileX") / 2; ++j)
                    {
                        if (j < 0 || i < 0 || i >= resource->getNumOfTileY() || j >= resource->getNumOfTileX()) continue;
                        
                        auto left = worldToLocal(_debugOrigin, tiles[i][j].getPosition() + Vec2(-64, 0));
                        auto bottom = worldToLocal(_debugOrigin, tiles[i][j].getPosition() + Vec2(0, -64));
                        auto right = worldToLocal(_debugOrigin, tiles[i][j].getPosition() + Vec2(64, 0));
                        
                        drawList->AddLine(ImVec2(right.x, right.y), ImVec2(bottom.x, bottom.y), ImColor(ImVec4(1.0, 1.0, 1.0, 0.2)));
                        drawList->AddLine(ImVec2(bottom.x, bottom.y), ImVec2(left.x, left.y), ImColor(ImVec4(1.0, 1.0, 1.0, 0.2)));
                    }
                }
            }
            
            if (_isLocationViewOn)
            {
                const auto& locations = game->getGameResource()->getLocations();
                for (const auto& location : locations)
                {
                    const auto& name = location.first;
                    const auto& rect = location.second;
                    
                    Vec2 rectOrigin = worldToLocal(_debugOrigin, rect.origin);
                    Vec2 rectDest = worldToLocal(_debugOrigin, rect.origin + rect.size);
                    
                    drawList->AddRect(ImVec2(rectOrigin.x, rectOrigin.y),
                                      ImVec2(rectDest.x, rectDest.y),
                                      ImColor(ImVec4(1.0, 1.0, 1.0, 0.6)));
                    drawList->AddRectFilled(ImVec2(rectOrigin.x, rectOrigin.y),
                                            ImVec2(rectDest.x, rectDest.y),
                                            ImColor(ImVec4(0.2, 0.2, 0.8, 0.2)));
                    
                    drawList->AddText(ImVec2(rectOrigin.x, rectOrigin.y), ImColor(ImVec4(1.0, 1.0, 0.0, 1.0)), name.c_str());
                }
            }
            
            if (_isGraphNodeViewOn)
            {
                auto graph = game->getGraph();
                auto nodes = graph->getNodes();
                for(const auto& node : nodes)
                {
                    auto playerPosition = game->getPlayerPtr()->getWorldPosition();
                    if ( playerPosition.distanceSquared(node.getPos()) < 200 * 200 )
                    {
                        Vec2 rectOrigin = worldToLocal(_debugOrigin, node.getPos());
                        Vec2 rectDest = rectOrigin + Vec2(3, 3);
                        drawList->AddRect(ImVec2(rectOrigin.x, rectOrigin.y), ImVec2(rectDest.x, rectDest.y), ImColor(ImVec4(1.0, 1.0, 1.0, 0.6)));
                        
                        auto resource = game->getGameResource();
                        const auto& edges = graph->getEdges(node.getIndex());
                        for(const auto& edge : edges)
                        {
                            auto from = numberToIndex(edge.getFrom(), resource->getNumOfTileX(), DUMMY_TILE_SIZE);
                            auto to = numberToIndex(edge.getTo(), resource->getNumOfTileX(), DUMMY_TILE_SIZE);
                            auto fromPosition = indexToPosition(from.first, from.second, resource->getTileWidth(), resource->getTileHeight(), DUMMY_TILE_SIZE);
                            auto toPosition = indexToPosition(to.first, to.second, resource->getTileWidth(), resource->getTileHeight(), DUMMY_TILE_SIZE);
                            
                            Vec2 fromOrigin = worldToLocal(_debugOrigin, fromPosition);
                            Vec2 toOrigin = worldToLocal(_debugOrigin, toPosition);
                            drawList->AddLine(ImVec2(fromOrigin.x, fromOrigin.y), ImVec2(toOrigin.x, toOrigin.y), ImColor(ImVec4(1.0, 1.0, 0.0, 0.2)));
                        }
                    }
                }
            }
            
            if ( _isPhysicsShape || _isPhysicsAABB )
            {
                game->getPhysicsManager()->GetPhysicsWorld()->DrawDebugData();
            }
            
            if ( _isNavGraphOn )
            {
                auto entities = game->getEntityManager()->getEntities();
                for( const auto& entity : entities )
                {
                    auto ent = entity.second;
                    if ( isMasked(ent->getFamilyMask(), FamilyMask::HUMAN_BASE))
                    {
                        auto human = static_cast<HumanBase*>(ent);
                        auto renderPath = human->getPathPlanner()->getRenderPath();
                        if ( human->isAlive() && renderPath )
                        {
                            for ( auto& path : *renderPath )
                            {
                                Vec2 s = path.getSource();
                                Vec2 d = path.getDestination();
                                
                                Vec2 fromOrigin = worldToLocal(_debugOrigin, s);
                                Vec2 toOrigin = worldToLocal(_debugOrigin, d);
                                
                                drawList->AddLine(ImVec2(fromOrigin.x, fromOrigin.y), ImVec2(toOrigin.x, toOrigin.y), ImColor(ImVec4(1.0, 0.0, 1.0, 1.0)));
                            }
                        }
                    }
                }
            }
            
            if ( _isGoalDescOn )
            {
                auto entities = game->getNeighborsEntities(game->getPlayerPtr()->getWorldPosition(), cocos2d::Rect(0, 0, GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT));
                for( const auto& ent : entities )
                {
                    if ( isMasked(ent->getFamilyMask(), FamilyMask::HUMAN_BASE) )
                    {
                        auto human = static_cast<HumanBase*>(ent);
                        auto brain = human->getBrain();
                        if ( human->isAlive() && brain )
                        {
                            _lines = 0;
                            drawGoalDatas(0, brain->getGoalThink());
                        }
                    }
                }
            }
            
        }
    }
    ImGui::End();
    
    if (_isGameStarted)
    {
        auto game = _gameLayer->getGame();
        if (game->isGameEnded())
            opened = false;
    }
    
    if (!opened)
    {
        closeLayer();
    }
    
    
    // statistic
    const char* tabNames[] = {"status", "profiler", "debug"};
    const int numTabs = 3;
    const char* tabTooltips[numTabs] = {"fps, draw calls, vertices...", "cpu usages...", "set draw nodes (on or off)..."};
    static int selectedTab = 0;
    static int optionalHoveredTab = 0;
    
    if ( _isGameStarted )
    {
        ImGui::SetNextWindowPos(ImVec2(100,100), ImGuiSetCond_Once);
        ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiSetCond_Once);
        ImGui::Begin("statistic", NULL, ImGuiWindowFlags_ShowBorders);
        
        auto drawList = ImGui::GetWindowDrawList();
        auto game = _gameLayer->getGame();
        
        ImGui::TabLabels(numTabs, tabNames, selectedTab, tabTooltips, false, &optionalHoveredTab);
        
        if ( selectedTab == 0 )
        {
            ImGui::BeginChild("##diagram wrapper", ImVec2(0, 200), false);
            
            auto origin = ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + ImGui::GetWindowSize().y);
            _profileRenderOrigin = ImVec2(origin.x, origin.y);
            
            const int HORIZONTAL_LINE_LENGTH = ImGui::GetWindowSize().x - 15;
            const int VERTICAL_LINE_LENGTH = ImGui::GetWindowSize().y - 10;
            const float HORIZONTAL_SLICE = 50.0f;
            
            // horizontal line
            drawList->AddLine(ImVec2(origin.x + 5, origin.y),
                              ImVec2(origin.x + ImGui::GetWindowSize().x - 10, origin.y),
                              ImColor(BLACK));
            
            for(int i = 0 ; i < HORIZONTAL_SLICE ; ++ i)
            {
                drawList->AddLine(ImVec2(origin.x + 5 + ((i + 1) / HORIZONTAL_SLICE) * HORIZONTAL_LINE_LENGTH, origin.y),
                                  ImVec2(origin.x + 5 + ((i + 1) / HORIZONTAL_SLICE) * HORIZONTAL_LINE_LENGTH, origin.y - 3), ImColor(BLACK));
            }
            
            // vertical line
            drawList->AddLine(ImVec2(origin.x + 5, origin.y),
                              ImVec2(origin.x + 5, origin.y - ImGui::GetWindowSize().y + 10),
                              ImColor(BLACK));
            
            ImGui::EndChild();
            
            if ( _profileStatus.size() > 1 )
            {
                if ( _profileStatus.size() > HORIZONTAL_SLICE + 1 ) _profileStatus.pop_front();
                
                drawList->AddRectFilled(ImVec2(origin.x, origin.y + 22), ImVec2(origin.x + 10, origin.y + 17), ImColor(GRAYLISH_GREEN));
                drawList->AddRectFilled(ImVec2(origin.x, origin.y + 42), ImVec2(origin.x + 10, origin.y + 37), ImColor(GRAYLISH_RED));
                drawList->AddRectFilled(ImVec2(origin.x, origin.y + 62), ImVec2(origin.x + 10, origin.y + 57), ImColor(GRAYLISH_BLUE));
                
                drawList->AddText(ImVec2(origin.x + 15, origin.y + 10), ImColor(BLACK), std::string("FPS: " + _to_string(_profileStatus.back().fps)).c_str());
                drawList->AddText(ImVec2(origin.x + 15, origin.y + 30), ImColor(BLACK), std::string("Vertices: " + _to_string(_profileStatus.back().numOfVertices)).c_str());
                drawList->AddText(ImVec2(origin.x + 15, origin.y + 50), ImColor(BLACK), std::string("Draw Calls: " + _to_string(_profileStatus.back().drawCalls)).c_str());
                
                drawList->AddText(ImVec2(origin.x + 5 + (_profileStatus.size() / HORIZONTAL_SLICE) * HORIZONTAL_LINE_LENGTH - 20,
                                         origin.y - (_profileStatus.back().fps / 60.0f) * VERTICAL_LINE_LENGTH + 5),
                                  ImColor(GRAYLISH_GREEN), _to_string(_profileStatus.back().fps).c_str());
                
                drawList->AddText(ImVec2(origin.x + 5 + (_profileStatus.size() / HORIZONTAL_SLICE) * HORIZONTAL_LINE_LENGTH - 40,
                                         origin.y - (_profileStatus.back().numOfVertices / 20000.0f) * VERTICAL_LINE_LENGTH - 15),
                                  ImColor(GRAYLISH_RED), _to_string(_profileStatus.back().numOfVertices).c_str());
                
                drawList->AddText(ImVec2(origin.x + 5 + (_profileStatus.size() / HORIZONTAL_SLICE) * HORIZONTAL_LINE_LENGTH - 30,
                                         origin.y - (_profileStatus.back().drawCalls / 200.0f) * VERTICAL_LINE_LENGTH - 15),
                                  ImColor(GRAYLISH_BLUE), _to_string(_profileStatus.back().drawCalls).c_str());
                
                int i = 0;
                for(auto iter = std::begin(_profileStatus) ; iter != prev(std::end(_profileStatus), 1) ; ++ iter, ++ i)
                {
                    drawList->AddLine(ImVec2(origin.x + 5 + (i / HORIZONTAL_SLICE) * HORIZONTAL_LINE_LENGTH, origin.y - (iter->fps / 60.0f) * VERTICAL_LINE_LENGTH),
                                      ImVec2(origin.x + 5 + ((i + 1) / HORIZONTAL_SLICE) * HORIZONTAL_LINE_LENGTH, origin.y - (next(iter, 1)->fps / 60.0f) * VERTICAL_LINE_LENGTH),
                                      ImColor(GRAYLISH_GREEN));
                    
                    drawList->AddLine(ImVec2(origin.x + 5 + (i / HORIZONTAL_SLICE) * HORIZONTAL_LINE_LENGTH, origin.y - (iter->numOfVertices / 20000.0f) * VERTICAL_LINE_LENGTH),
                                      ImVec2(origin.x + 5 + ((i + 1) / HORIZONTAL_SLICE) * HORIZONTAL_LINE_LENGTH, origin.y - (next(iter, 1)->numOfVertices / 20000.0f) * VERTICAL_LINE_LENGTH),
                                      ImColor(GRAYLISH_RED));
                    
                    drawList->AddLine(ImVec2(origin.x + 5 + (i / HORIZONTAL_SLICE) * HORIZONTAL_LINE_LENGTH, origin.y - (iter->drawCalls / 200.0f) * VERTICAL_LINE_LENGTH),
                                      ImVec2(origin.x + 5 + ((i + 1) / HORIZONTAL_SLICE) * HORIZONTAL_LINE_LENGTH, origin.y - (next(iter, 1)->drawCalls / 200.0f) * VERTICAL_LINE_LENGTH),
                                      ImColor(GRAYLISH_BLUE));
                    
                }
            }
        }
        
        else if ( selectedTab == 1 )
        {
            if ( !_profileCPU.empty() )
            {
                ImGui::Separator();
                drawProfileCPUs(_profileCPU.back());
            }
        }
        else if ( selectedTab == 2)
        {
            if (!game->isGameEnded())
            {
                if (ImGui::TreeNode("normal"))
                {
                    ImGui::Checkbox("player info", &_isPlayerInfo);
                    ImGui::Checkbox("grid", &_isGridOn);
                    ImGui::Checkbox("location", &_isLocationViewOn);
                    ImGui::Checkbox("graph", &_isGraphNodeViewOn);
                    ImGui::TreePop();
                }
                
                if ( ImGui::TreeNode("physics") )
                {
                    if ( ImGui::Checkbox("shape", &_isPhysicsShape) )
                    {
                        if ( _isPhysicsShape ) AppendFlags(b2Draw::e_shapeBit);
                        else ClearFlags(b2Draw::e_shapeBit);
                    }
                    if ( ImGui::Checkbox("aabb", &_isPhysicsAABB) )
                    {
                        if ( _isPhysicsAABB ) AppendFlags(b2Draw::e_aabbBit);
                        else ClearFlags(b2Draw::e_aabbBit);
                    }
                    
                    ImGui::TreePop();
                }
                
                if ( ImGui::TreeNode("ai") )
                {
                    ImGui::Checkbox("path", &_isNavGraphOn);
                    ImGui::Checkbox("goal", &_isGoalDescOn);
                    
                    ImGui::TreePop();
                }
            }
            
        }
        
        ImGui::End();
        
        // logger
        ImGui::SetNextWindowPos(ImVec2(100,600), ImGuiSetCond_Once);
        ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiSetCond_Once);
        ImGui::Begin("logger", NULL, ImGuiWindowFlags_ShowBorders);
        ImGui::TextUnformatted(game->getLogString().c_str());
        if (game->isLogAdded()) ImGui::SetScrollHere(1.0f);
        game->isLogAdded() = false;
        ImGui::End();

    }
    
    if ( !_tcpSession.isQueueEmpty() )
    {
        network::Packet* packet = nullptr;
        _tcpSession.dequeue(packet);
        packet->decode();
        
        if ( _isGameStarted )
        {
            switch ( packet->type() )
            {
                case network::PacketType::PROFILE_INFO_FLATBUFFERS:
                {
                    // status
                    ProfileStatus stat;
                    stat.fps = ImGui::GetIO().Framerate;
                    stat.drawCalls = Director::getInstance()->getRenderer()->getDrawnBatches();
                    stat.numOfVertices = Director::getInstance()->getRenderer()->getDrawnVertices();
                    _profileStatus.push_back(stat);
                    
                    // cpu usage
                    auto obj = realtrick::profiler::GetData(packet->body());
                    auto mainloop = obj->main_loop();
                    
                    ProfileCPU mainloopBlock;
                    mainloopBlock.calls = mainloop->calls();
                    mainloopBlock.avgTime = mainloop->avg_time();
                    mainloopBlock.minTime = mainloop->min_time();
                    mainloopBlock.maxTime = mainloop->max_time();
                    mainloopBlock.name = mainloop->name()->str();
                    mainloopBlock.usage = mainloop->usage();
                    for( auto child = mainloop->children()->begin() ; child != mainloop->children()->end() ; ++ child )
                        fillProfileCPUDatas(mainloopBlock.children, *child);
                    
                    _profileCPU.push_back(mainloopBlock);
                    
                    break;
                }
                default: break;
            }
        }
        
        delete packet;
        packet = nullptr;
    }
}


void SimulatorLayer::playGame()
{
    _gameLayer = realtrick::client::DummyScene::create(this);
    addChild(_gameLayer);
}


void SimulatorLayer::closeLayer()
{
    _profileStatus.clear();
    _gameLayer->removeFromParentAndCleanup(true);
    _imguiLayer->stopGame();
}


cocos2d::Vec2 SimulatorLayer::worldToLocal(const cocos2d::Vec2& p)
{
    auto game = _gameLayer->getGame();
    auto transformedVector =  p - game->getPlayerPtr()->getWorldPosition();
    auto zoomScale = game->getRenderingSysetm()->getZoomScale();
    return cocos2d::Vec2(transformedVector.x * zoomScale.x, transformedVector.y * zoomScale.y);
}


cocos2d::Vec2 SimulatorLayer::worldToLocal(const cocos2d::Vec2& origin, const cocos2d::Vec2& p)
{
    auto local = worldToLocal(p);
    return cocos2d::Vec2(origin.x + local.x + GAME_SCREEN_WIDTH / 2, origin.y - local.y + GAME_SCREEN_HEIGHT / 2);
}


void SimulatorLayer::fillProfileCPUDatas(std::vector<ProfileCPU>& parent, const realtrick::profiler::Element* elem)
{
    ProfileCPU block;
    block.calls = elem->calls();
    block.avgTime = elem->avg_time();
    block.minTime = elem->min_time();
    block.maxTime = elem->max_time();
    block.name = elem->name()->str();
    block.usage = elem->usage();
    
    for( auto child = elem->children()->begin() ; child != elem->children()->end() ; ++ child )
        fillProfileCPUDatas(block.children, *child);
    
    parent.push_back(block);
}


void SimulatorLayer::drawProfileCPUs(const ProfileCPU& data)
{
    if ( ImGui::TreeNode(data.name.c_str()) )
    {
        float calls = data.calls / (float)_profileStatus.back().fps;
        ImGui::SameLine(); ImGui::TextColored(ImColor(GRAYLISH_RED), "%5.2f%s", data.usage, "(%)");
        ImGui::SameLine(); ImGui::TextColored(ImColor(GRAYLISH_RED), "%3d%s", (int)calls, "(calls)");
        ImGui::SameLine(); ImGui::TextColored(ImColor(GRAYLISH_RED), "%7.3f%s", data.avgTime * calls / 1000.0f, "(ms)");
        for(auto child = std::begin(data.children) ; child != std::end(data.children) ; ++ child)
            drawProfileCPUs(*child);
        
        ImGui::TreePop();
    }
}


void SimulatorLayer::drawGoalDatas(int blanks, GoalBase* goal)
{
    auto drawList = ImGui::GetWindowDrawList();
    
    ImColor col;
    
    if ( !goal->isCompositeGoal() ) col = ImColor(ImVec4(1.0, 1.0, 1.0, 1.0));
    else
    {
        if ( static_cast<GoalCompositeBase*>(goal)->getSubGoals().empty() )
            col = ImColor(ImVec4(1.0, 1.0, 1.0, 1.0));
        else
            col = ImColor(ImVec4(0.5, 0.5, 0.5, 1.0));
    }
    
    Vec2 origin = worldToLocal(_debugOrigin, goal->getOwner()->getWorldPosition());
    drawList->AddText(ImVec2(origin.x + blanks * 4, origin.y + _lines * 20), col, goal->getGoalName().c_str());
    
    if ( goal->isCompositeGoal() )
    {
        auto compositeGoal = static_cast<GoalCompositeBase*>(goal);
        for(const auto& subGoal : compositeGoal->getSubGoals() )
        {
            _lines++;
            drawGoalDatas(blanks + 1, subGoal);
        }
    }
}


bool SimulatorLayer::isNeighborPoint(const cocos2d::Vec2& p)
{
    auto game = _gameLayer->getGame();
    auto worldPosition = game->getPlayerPtr()->getWorldPosition();
    return (p.distanceSquared(worldPosition) < 300 * 300);
}


//
void SimulatorLayer::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    auto drawList = ImGui::GetWindowDrawList();
    for (size_t i = 0; i < vertexCount; i++)
    {
        size_t j = (i + 1) % vertexCount;
        if ( isNeighborPoint(toVec(vertices[i])) && isNeighborPoint(toVec(vertices[j])) )
        {
            auto a = worldToLocal(_debugOrigin, toVec(vertices[i]));
            auto b = worldToLocal(_debugOrigin, toVec(vertices[j]));
            drawList->AddLine(ImVec2(a.x, a.y), ImVec2(b.x, b.y), ImColor(ImVec4(1.0, 1.0, 0.0, 0.5)));
        }
    }
}

/// Draw a solid closed polygon provided in CCW order.
void SimulatorLayer::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    DrawPolygon(vertices, vertexCount, color);
}


void SimulatorLayer::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
    auto drawList = ImGui::GetWindowDrawList();
    auto rad = radius * _gameLayer->getGame()->getRenderingSysetm()->getZoomScale().x;
    if ( isNeighborPoint(toVec(center)) )
    {
        auto pos = worldToLocal(_debugOrigin, toVec(center));
        drawList->AddCircle(ImVec2(pos.x, pos.y), rad, ImColor(ImVec4(1.0, 0.0, 1.0, 0.7)), 20, 0.0f);
    }
}


void SimulatorLayer::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
    DrawCircle(center, radius, color);
}


void SimulatorLayer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    auto drawList = ImGui::GetWindowDrawList();
    if ( isNeighborPoint(toVec(p1)) && isNeighborPoint(toVec(p2)) )
    {
        auto a = worldToLocal(_debugOrigin, toVec(p1));
        auto b = worldToLocal(_debugOrigin, toVec(p2));
        drawList->AddLine(ImVec2(a.x, a.y), ImVec2(b.x, b.y), ImColor(ImVec4(1.0, 1.0, 0.0, 0.5)));
    }
}


void SimulatorLayer::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
    auto drawList = ImGui::GetWindowDrawList();
    if ( isNeighborPoint(toVec(p)) && isNeighborPoint(toVec(b2Vec2(p.x + size, p.y + size))) )
    {
        Vec2 rectOrigin = worldToLocal(_debugOrigin, toVec(p));
        Vec2 rectDest = worldToLocal(_debugOrigin, Vec2(p.x + size, p.y + size));
        drawList->AddRectFilled(ImVec2(rectOrigin.x, rectOrigin.y),
                                ImVec2(rectDest.x, rectDest.y),
                                ImColor(ImVec4(0.2, 0.2, 0.8, 0.2)));
    }
}



