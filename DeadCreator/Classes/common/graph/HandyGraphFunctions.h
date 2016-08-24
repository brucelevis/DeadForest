#ifndef GRAPH_FUNCS
#define GRAPH_FUNCS
//-----------------------------------------------------------------------------
//
//  Name:   HandyGraphFunctions.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   As the name implies, some useful functions you can use with your
//          graphs. 
//          For the function templates, make sure your graph interface complies
//          with the SparseGraph class
//-----------------------------------------------------------------------------
#include <iostream>

#include "GraphAlgorithms.h"
#include "AStarHeuristicPolicies.h"
#include "GraphNodeTypes.h"
#include "GraphEdgeTypes.h"
#include "PathEdge.h"
#include "Physics.hpp"
#include "TileHelperFunctions.hpp"
#include "GraphEdgeTypes.h"

namespace
{
	
	//--------------------------- validNeighbour -----------------------------
	//
	//  returns true if x,y is a valid position in the map
	//------------------------------------------------------------------------
	
	bool validNeighbour(int x, int y, int num_cellsX, int num_cellsY)
	{
		return !((x < 0) || (x >= num_cellsX) || (y < 0) || (y >= num_cellsY));
	}

	bool isValidIndex(int j, int i, int numX, int numY)
	{
		return 0 <= j && j < numX && 0 <= i && i < numY;
	}

	template <typename Graph, typename Obstacles>
	void generateIsometricGridGraph(
		Graph& graph, const Obstacles& obs, int numX, int numY, float tileX, float tileY, int numOfDummy)
	{
		cocos2d::Vec2 pos;
		int tileNumX = numX + numOfDummy * 2;
		int tileNumY = numY * 2 + numOfDummy * 4;
		for (int i = 0; i < tileNumY; i++)
		{
			for (int j = 0; j < tileNumX; j++)
			{
				pos = indexToPosition(j, i, tileX, tileY, numOfDummy);
				graph.addNode(realtrick::NavGraphNode(graph.getNextFreeNodeIndex(), pos));
			}
		}

		for (auto n = std::begin(graph.getNodes()); n != std::end(graph.getNodes()); n++)
		{
			int from = n->getIndex();
			auto idx_pair = numberToIndex(from, numX, numOfDummy);

			auto adj = getNeighborTiles(idx_pair.first, idx_pair.second);

			for (auto& e : adj)
			{
				int to_j = e.first;
				int to_i = e.second;

				if (!isValidIndex(to_j, to_i, numX, numY))
					continue;

				int to = indexToNumber(to_j, to_i, numX, numOfDummy);

				graph.getNode(from).getPos();
				graph.getNode(to).getPos();

				bool collide = false;
				for (auto& e : obs)
				{
					realtrick::Segment seg(graph.getNode(from).getPos(), graph.getNode(to).getPos());
					if (realtrick::physics::intersect(e, seg))
					{
						collide = true;
						break;
					}
				}

				if(!collide)
				{
                    graph.addEdge(realtrick::NavGraphEdge(from, to, graph
						.getNode(from).getPos().getDistance(graph.getNode(to).getPos())));
				}
			}
		}
	}

	template <typename Graph>
	void renderGraph(const Graph& graph, cocos2d::DrawNode* panel, const cocos2d::Vec2& camPos)
	{
		for (auto n = std::begin(graph.getNodes()); n != std::end(graph.getNodes()); n++)
		{
			panel->drawCircle(n->getPos() - camPos, 10, M_PI * 2, 40, false, cocos2d::Color4F::YELLOW);
			int idx = n->getIndex();

			for (auto e = std::begin(graph.getEdges(idx)); e != std::end(graph.getEdges(idx)); e++)
			{
				panel->drawLine(
					graph.getNode(e->getFrom()).getPos() - camPos,
					graph.getNode(e->getTo()).getPos() - camPos,
					cocos2d::Color4F::GRAY);
			}
		}
	}

	void renderPath(const std::list<realtrick::PathEdge>& edges, cocos2d::DrawNode* panel)
	{
		for (auto e : edges)
		{
			panel->drawSolidCircle(e.getSource(), 12, M_PI * 2, 40, cocos2d::Color4F::RED);
			panel->drawSolidCircle(e.getDestination(), 12, M_PI * 2, 40, cocos2d::Color4F::RED);

			panel->drawLine(
				e.getSource(),
				e.getDestination(),
				cocos2d::Color4F::BLUE);
		}
	}
	
}

#endif
