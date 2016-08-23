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
#include "GraphAlgorithms.h"
#include "SparseGraph.h"
#include "PathEdge.h"
#include "Game.hpp"

namespace realtrick
{
	namespace client
	{
		class EntityBase;

		class PathPlanner
		{
		public:
			//for ease of use typdef the graph edge/node types used by the navgraph
			typedef typename Game::Graph		Graph;
			typedef typename Graph::Edge		Edge;
			typedef typename Graph::Node		Node;
			typedef std::list<PathEdge>			Path;

		public:

			PathPlanner(const Graph& graph, EntityBase* const owner);

			~PathPlanner() {}

			//called by an agent after it has been notified that a search has terminated
			//successfully. The method extracts the path from _current_algorithm, adds
			//additional edges appropriate to the search type and returns it as a list of
			//PathEdges.
			Path getPath(cocos2d::Vec2 source, cocos2d::Vec2 destination, int tileX, int tileY, int numOfDummy);

			// used to retrieve the position of a graph node from its index. 
			// (takes into account the enumerations 'non_graph_source_node' and 'non_graph_target_node')
			cocos2d::Vec2	getNodePosition(int idx) const { return _graph.getNode(idx).getPos(); }


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
			EntityBase* const					_owner;

			//a reference to the navgraph
			const Graph&						_graph;
		};

	}
}
#endif

