#ifndef PATHPLANNER_H
#define PATHPLANNER_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   PathPlanner.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   class to handle the creation of paths through a navigation graph
//-----------------------------------------------------------------------------

#include <list>
#include <memory>
#include "cocos2d.h"
#include "GraphAlgorithms.hpp"
#include "SparseGraph.hpp"
#include "PathEdge.hpp"
#include "Game.hpp"

namespace realtrick
{
	namespace client
	{
		class HumanBase;

		class PathPlanner
		{
		public:
			//for ease of use typdef the graph edge/node types used by the navgraph
			typedef typename Game::Graph		Graph;
			typedef typename Graph::Edge		Edge;
			typedef typename Graph::Node		Node;
			typedef std::list<PathEdge>			Path;

		public:

			PathPlanner(const Graph& graph, HumanBase* const owner);

			~PathPlanner() {}

		
			//called by an agent after it has been notified that a search has terminated
			//successfully. The method extracts the path from _current_algorithm, adds
			//additional edges appropriate to the search type and returns it as a list of
			//PathEdges.
			void generatePath(
				cocos2d::Vec2 source,
				cocos2d::Vec2 destination,
				int numOfTileX,
				int numOfTileY,
				int tileWidth,
				int tileHeight,
				int numOfDummy);

			// used to retrieve the position of a graph node from its index. 
			// (takes into account the enumerations 'non_graph_source_node' and 'non_graph_target_node')
			cocos2d::Vec2	getNodePosition(int idx) const { return _graph.getNode(idx).getPos(); }

			int findAvailableNeighberNode(int node, int numOfTileX, int numOfDummy);

			double calculateTimeToReachPosition(cocos2d::Vec2 pos)const;

			int getClosestNodeToPosition(cocos2d::Vec2 pos)const;
			
			bool requestPathToPosition(cocos2d::Vec2 targetPos);

			Path getPath() { return _path; }

		private:

			//smooths a path by removing extraneous edges. 
			//(may not remove all extraneous edges)
			void  smoothPathEdgesQuick(Path& path);

			//smooths a path by removing extraneous edges. 
			//(removes *all* extraneous edges)
			void  smoothPathEdgesPrecise(Path& path);

			//for legibility
			enum { NO_CLOSEST_NODE_FOUND = -1 };

			//A pointer to the owner of this class, only references.
			HumanBase* const					_owner;

			//a reference to the navgraph
			const Graph&						_graph;

			Path	_path;
		};

	}
}
#endif

