#include "PathPlanner.h"
#include "EntityBase.hpp"
#include "HandyGraphFunctions.h"
#include "TileHelperFunctions.hpp"

using namespace realtrick;
using namespace client;
	

//------------------------------ ctor -----------------------------------------
//-----------------------------------------------------------------------------
PathPlanner::PathPlanner(const Graph& graph, EntityBase* const owner)
	:
	_owner(owner),
	_graph(graph)
{}


//----------------------------- getPath ------------------------------------
//
//  called by an agent after it has been notified that a search has terminated
//  successfully. The method extracts the path from _current_algorithm, adds
//  additional edges appropriate to the search type and returns it as a list of
//  PathEdges.
//-----------------------------------------------------------------------------


typename PathPlanner::Path PathPlanner::getPath(
	cocos2d::Vec2 source,
	cocos2d::Vec2 destination,
	int numOfTileX,
	int numOfTileY,
	int tileWidth,
	int tileHeight,
	int numOfDummy)
{
	CCLOG("source vec2 %f %f", source.x, source.y);

	std::pair<int, int> source_idx = getFocusedTileIndex(source, tileWidth, tileHeight, numOfDummy);
	std::pair<int, int> desti_idx = getFocusedTileIndex(destination, tileWidth, tileHeight, numOfDummy);



	CCLOG("source idx %d %d", source_idx.first, source_idx.second);

	int source_node_num = indexToNumber(source_idx.first, source_idx.second, numOfTileX, numOfDummy);
	int desti_node_num = indexToNumber(desti_idx.first, desti_idx.second, numOfTileX, numOfDummy);

	CCLOG("number %d %d", source_node_num, desti_node_num);
	SearchAStar<Graph, HeuristicEuclid> search(_graph, source_node_num, desti_node_num);

	PathEdge source_to_path(source, _graph.getNode(source_node_num).getPos());
	PathEdge path_to_destination(_graph.getNode(desti_node_num).getPos(), destination);

	auto path = search.getPathAsPathEdges();
	path.push_front(source_to_path);
	path.push_back(path_to_destination);

	if (path.size() == 0)
		CCLOG("path is zero");

	//smooth paths if required
	if (false)
	{
		smoothPathEdgesQuick(path);
	}

	if (false)
	{
		smoothPathEdgesPrecise(path);
	}

	return path;
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