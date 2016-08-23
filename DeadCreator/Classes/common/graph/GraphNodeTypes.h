#ifndef GRAPH_NODE_TYPES_H
#define GRAPH_NODE_TYPES_H
//-----------------------------------------------------------------------------
//
//  Name:   GraphNodeTypes.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   Node classes to be used with graphs
//-----------------------------------------------------------------------------
#include <list>
#include <sstream>
#include "cocos2d.h"

namespace realtrick
{


	class GraphNode
	{
	public:

		GraphNode() :_index(-1) {}
		explicit GraphNode(int idx) :_index(idx) {}

		virtual ~GraphNode()
		{}

		int  getIndex() const { return _index; }
		void setIndex(int new_index) { _index = new_index; }

	protected:

		//every node has an index. A valid index is >= 0
		int        _index;

	};



	//-----------------------------------------------------------------------------
	//
	//  Graph node for use in creating a navigation graph.This node contains
	//  the position of the node and a pointer to a BaseGameEntity... useful
	//  if you want your nodes to represent health packs, gold mines and the like
	//-----------------------------------------------------------------------------
	class NavGraphNode : public GraphNode
	{
	protected:

		//the node's position
		cocos2d::Vec2     _position;

	public:

		//ctors
		NavGraphNode() {}

		NavGraphNode(int idx, cocos2d::Vec2 pos)
			:
			GraphNode(idx),
			_position(pos)
		{}

		virtual ~NavGraphNode() override
		{}

		cocos2d::Vec2   getPos() const { return _position; }
		void			setPos(cocos2d::Vec2 new_position) { _position = new_position; }
	};

	
}
#endif
