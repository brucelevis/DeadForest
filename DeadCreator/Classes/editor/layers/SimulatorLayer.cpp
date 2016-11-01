//
//  SimulatorLayer.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 28..
//
//


#include "DummyScene.hpp"
#include "EditScene.hpp"
#include "SimulatorLayer.hpp"
using namespace realtrick::editor;

#include "Game.hpp"
#include "Camera2D.hpp"
#include "GameResource.hpp"
#include "RenderingSystem.hpp"
#include "SparseGraph.h"
using namespace realtrick;
using namespace realtrick::client;
using namespace cocos2d;

#include "profiling_schema_generated.h"


void SimulatorLayer::showLayer(bool& opened)
{
    receiveProfileData();
    
	static bool isStatusOn = true;
	static bool isPlayerInfo = true;
	static bool isGridOn = false;
	static bool isLocationViewOn = false;
	static bool isGraphNodeViewOn = false;

    static bool isPhysicsShape = false;
    static bool isPhysicsJoint = false;
    static bool isPhysicsAABB = false;
    static bool isPhysicsPair = false;
    static bool isPhysicsCenterOfMass = false;

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

			if (isStatusOn)
			{
				static auto renderer = Director::getInstance()->getRenderer();

				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.00, 1.00, 1.00, 1.00));
				ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
				ImGui::Text("Vertices: %d", static_cast<int>(renderer->getDrawnVertices()));
				ImGui::Text("Batches: %d", static_cast<int>(renderer->getDrawnBatches()));
                ImGui::Text("Elapsed Time: %d (sec)", static_cast<int>(game->getElapsedTime()));
                
				ImGui::PopStyleColor();
			}

			if (isPlayerInfo)
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

			if (isGridOn)
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

			if (isLocationViewOn)
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

			if (isGraphNodeViewOn)
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
            
            if ( isPhysicsShape || isPhysicsJoint || isPhysicsAABB || isPhysicsPair || isPhysicsCenterOfMass )
            {
                game->getPhysicsWorld()->DrawDebugData();
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

	if (!opened) closeLayer();

	// setting layer
	if (_isGameStarted)
	{
		auto game = _gameLayer->getGame();
		if (!game->isGameEnded())
		{
			ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiSetCond_Once);
			ImGui::Begin("statistic", NULL, ImGuiWindowFlags_ShowBorders);

			if (ImGui::TreeNode("debug"))
			{
				ImGui::Checkbox("status", &isStatusOn);
				ImGui::Checkbox("player info", &isPlayerInfo);
				ImGui::Checkbox("grid", &isGridOn);
				ImGui::Checkbox("location", &isLocationViewOn);
				ImGui::Checkbox("graph", &isGraphNodeViewOn);
				ImGui::TreePop();
			}
            
            if (ImGui::TreeNode("physics"))
            {
                if ( ImGui::Checkbox("shape", &isPhysicsShape) )
                {
                    if ( isPhysicsShape ) AppendFlags(b2Draw::e_shapeBit);
                    else ClearFlags(b2Draw::e_shapeBit);
                }
                if ( ImGui::Checkbox("aabb", &isPhysicsAABB) )
                {
                    if ( isPhysicsAABB ) AppendFlags(b2Draw::e_aabbBit);
                    else ClearFlags(b2Draw::e_aabbBit);
                }
                if ( ImGui::Checkbox("joint", &isPhysicsJoint) )
                {
                    if ( isPhysicsJoint ) AppendFlags(b2Draw::e_jointBit);
                    else ClearFlags(b2Draw::e_jointBit);
                }
                if ( ImGui::Checkbox("pair", &isPhysicsPair) )
                {
                    if ( isPhysicsPair ) AppendFlags(b2Draw::e_pairBit);
                    else ClearFlags(b2Draw::e_pairBit);
                }
                if ( ImGui::Checkbox("center of mass", &isPhysicsCenterOfMass) )
                {
                    if ( isPhysicsCenterOfMass ) AppendFlags(b2Draw::e_centerOfMassBit);
                    else ClearFlags(b2Draw::e_centerOfMassBit);
                }
                
                ImGui::TreePop();
            }
            
            if (ImGui::TreeNode("logger"))
            {
                ImGui::BeginChild("scrolling", ImVec2(0, 80), true, ImGuiWindowFlags_HorizontalScrollbar);
                ImGui::TextUnformatted(game->getLogString().c_str());
                if (game->isLogAdded()) ImGui::SetScrollHere(1.0f);
                game->isLogAdded() = false;
                ImGui::EndChild();
                ImGui::TreePop();
            }

			ImGui::End();
		}
	}
}


void SimulatorLayer::playGame()
{
	_gameLayer = realtrick::client::DummyScene::create(this);
	addChild(_gameLayer);
}


void SimulatorLayer::closeLayer()
{
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


void SimulatorLayer::receiveProfileData()
{
    
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



