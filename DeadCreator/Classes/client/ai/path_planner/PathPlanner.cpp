#include "PathPlanner.h"
#include "HumanBase.hpp"
#include "TileHelperFunctions.hpp"
#include "AStarHeuristicPolicies.h"
#include "Game.hpp"
#include "GameResource.hpp"
#include "Tileset.hpp"
#include "SparseGraph.h"

using namespace realtrick;
using namespace client;
	

//------------------------------ ctor -----------------------------------------
//-----------------------------------------------------------------------------
PathPlanner::PathPlanner(const Graph& graph, HumanBase* const owner)
	:
	_owner(owner),
	_graph(graph)
{}


//----------------------------- generatePath ------------------------------------
//
//  called by an agent after it has been notified that a search has terminated
//  successfully. The method extracts the path from _current_algorithm, adds
//  additional edges appropriate to the search type and returns it as a list of
//  PathEdges.
//-----------------------------------------------------------------------------


void PathPlanner::generatePath(
	cocos2d::Vec2 source,
	cocos2d::Vec2 destination,
	int numOfTileX,
	int numOfTileY,
	int tileWidth,
	int tileHeight,
	int numOfDummy)
{
	std::pair<int, int> source_idx = getFocusedTileIndex(source, tileWidth, tileHeight, numOfDummy);
	std::pair<int, int> desti_idx = getFocusedTileIndex(destination, tileWidth, tileHeight, numOfDummy);

	int source_node_num = indexToNumber(source_idx.first, source_idx.second, numOfTileX, numOfDummy);
	int desti_node_num = indexToNumber(desti_idx.first, desti_idx.second, numOfTileX, numOfDummy);

	bool source_has_edge = _graph.checkThisNodeHasEdge(source_node_num);
	bool desti_has_edge = _graph.checkThisNodeHasEdge(desti_node_num);

	if (!source_has_edge)
	{
		source_node_num = findAvailableNeighberNode(source_node_num, numOfTileX, numOfDummy);
	}

	if (!desti_has_edge)
	{
		desti_node_num = findAvailableNeighberNode(desti_node_num, numOfTileX, numOfDummy);
	}

	SearchAStar<Graph, HeuristicEuclid> search(_graph, source_node_num, desti_node_num);
	
	PathEdge source_to_path(source, _graph.getNode(source_node_num).getPos());
	PathEdge path_to_destination(_graph.getNode(desti_node_num).getPos(), destination);

	_path = search.getPathAsPathEdges();
	_path.push_front(source_to_path);
	_path.push_back(path_to_destination);

	if (_path.size() == 0)
		CCLOG("path is zero");

	//smooth paths if required
	if (false)
	{
		smoothPathEdgesQuick(_path);
	}

	if (false)
	{
		smoothPathEdgesPrecise(_path);
	}
}

//--------------------------- smoothPathEdgesQuick ----------------------------
//
//  smooths a path by removing extraneous edges.
//-----------------------------------------------------------------------------
void PathPlanner::smoothPathEdgesQuick(Path& path)
{
	if (path.size() == 0)
		return;

	//create a couple of iterators and point them at the front of the path
	Path::iterator e1(path.begin()), e2(path.begin());

	//increment e2 so it points to the edge following e1.
	++e2;

	//while e2 is not the last edge in the path, step through the edges checking
	//to see if the agent can move without obstruction from the source node of
	//e1 to the destination node of e2. If the agent can move between those 
	//positions then the two edges are replaced with a single edge.
	while (e2 != path.end())
	{
		//check for obstruction, adjust and remove the edges accordingly
		if ((e2->getBehavior() == Edge::NORMAL) 
			//&&_owner->canWalkBetween(e1->getSource(), e2->getDestination())
			)
		{
			e1->setDestination(e2->getDestination());
			e2 = path.erase(e2);
		}
		else
		{
			e1 = e2;
			++e2;
		}
	}
}

//----------------------- smoothPathEdgesPrecise ------------------------------
//
//  smooths a path by removing extraneous edges.
//-----------------------------------------------------------------------------

void PathPlanner::smoothPathEdgesPrecise(Path& path)
{
	if (path.size() == 0)
		return;

	//create a couple of iterators
	Path::iterator e1, e2;

	//point e1 to the beginning of the path
	e1 = path.begin();

	while (e1 != path.end())
	{
		//point e2 to the edge immediately following e1
		e2 = e1;
		++e2;

		//while e2 is not the last edge in the path, step through the edges
		//checking to see if the agent can move without obstruction from the 
		//source node of e1 to the destination node of e2. If the agent can move
		//between those positions then the any edges between e1 and e2 are
		//replaced with a single edge.
		while (e2 != path.end())
		{
			//check for obstruction, adjust and remove the edges accordingly
			if ((e2->getBehavior() == Edge::NORMAL)
				//&& _owner->canWalkBetween(e1->getSource(), e2->getDestination())
				)
			{
				e1->setDestination(e2->getDestination());
				e2 = path.erase(++e1, ++e2);
				e1 = e2;
				--e1;
			}

			else
			{
				++e2;
			}
		}

		++e1;
	}
}

int PathPlanner::findAvailableNeighberNode(int node, int numOfTileX, int numOfDummy)
{
	std::pair<int, int> p = numberToIndex(node, numOfTileX, numOfDummy);
	auto& v = getNeighborTiles(p.first, p.second);

	for (auto e : v)
	{
		int other_node = indexToNumber(e.first, e.second, numOfTileX, numOfDummy);
		if (_graph.checkThisNodeHasEdge(other_node))
		{
			return other_node;
		}
	}
	
	CCASSERT(false, "no available neighber node found!");
}

double PathPlanner::calculateTimeToReachPosition(cocos2d::Vec2 pos) const
{
	return (_owner->getWorldPosition() - pos).getLengthSq() / (_owner->getRunSpeed() * 100);
}

//------------------------ getClosestNodeToPosition ---------------------------
//
//  returns the index of the closest visible graph node to the given position
//-----------------------------------------------------------------------------
int PathPlanner::getClosestNodeToPosition(cocos2d::Vec2 pos) const
{
	int numOfTileX = _owner->getGame()->getGameResource()->getNumOfTileX();
	int numOfTileY = _owner->getGame()->getGameResource()->getNumOfTileY();
	float tileWidth = _owner->getGame()->getGameResource()->getTileWidth();
	float tileHeight = _owner->getGame()->getGameResource()->getTileHeight();
	int numOfDummy = DUMMY_TILE_SIZE;

	std::pair<int, int> idx_pair =  getFocusedTileIndex(
		pos,
		tileWidth,
		tileHeight,
		numOfDummy);

	if (_owner->getGame()->getGameResource()->getTileData()[idx_pair.second][idx_pair.first]
		.getTileType() != TileType::HILL)
		return indexToNumber(idx_pair.first, idx_pair.second, numOfTileX, numOfDummy);

	auto adj = getNeighborTiles(idx_pair.first, idx_pair.second);

	for (auto& e : adj)
	{
		int toJ = e.first;
		int toI = e.second;

		if (!isValidIndex(toJ, toI, numOfTileX, numOfTileY))
			continue;

		if (_owner->getGame()->getGameResource()->getTileData()[toI][toJ]
			.getTileType() != TileType::HILL)
			return indexToNumber(toJ, toI, numOfTileX, numOfDummy);
	}
	return NO_CLOSEST_NODE_FOUND;
}


//--------------------------- requestPathToPosition ------------------------------
//
//  Given a target, this method first determines if nodes can be reached from 
//  the  bot's current position and the target position. If either end point
//  is unreachable the method returns false. 
//
//  If nodes are reachable from both positions then an instance of the time-
//  sliced A* search is created and registered with the search manager. the
//  method then returns true.
//        
//-----------------------------------------------------------------------------
bool PathPlanner::requestPathToPosition(cocos2d::Vec2 targetPos)
{
	//if the target is walkable from the bot's position a path does not need to
	//be calculated, the bot can go straight to the position by ARRIVING at
	//the current waypoint

	cocos2d::Vec2 posLeft;
	cocos2d::Vec2 posRight;

	if (_owner->getGame()->isLOSOkay(_owner->getWorldPosition(), targetPos))
	{
		return false;
	}

	generatePath(
		_owner->getWorldPosition(), targetPos,
		_owner->getGame()->getGameResource()->getNumOfTileX(),
		_owner->getGame()->getGameResource()->getNumOfTileY(),
		_owner->getGame()->getGameResource()->getTileWidth(),
		_owner->getGame()->getGameResource()->getTileHeight(),
		DUMMY_TILE_SIZE);

	return true;
}