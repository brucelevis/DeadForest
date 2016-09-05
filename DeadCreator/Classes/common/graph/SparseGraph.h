#ifndef SPARSEGRAPH_H
#define SPARSEGRAPH_H
#pragma warning (disable:4786)
//------------------------------------------------------------------------
//
//  Name:   SparseGraph.h
//
//  Desc:   Graph class using the adjacency list representation.
//
//  Author: Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------
#include <vector>
#include <list>
#include <cassert>
#include <string>
#include <iostream>
#include <map>
#include "cocos2d.h"

namespace realtrick
{

	template <class NodeType, class EdgeType>
	class SparseGraph
	{
	public:

		//enable easy client access to the edge and node types used in the graph
		typedef EdgeType					Edge;
		typedef NodeType					Node;

		//a couple more typedefs to save my fingers and to help with the formatting
		//of the code on the printed page
		typedef std::vector<Node>		NodeVector;
		typedef std::list<Edge>			EdgeList;
		typedef std::vector<EdgeList>	EdgeListVector;

		//ctor
		SparseGraph() : _next_node_index(0)
		{}

		//dtor
		~SparseGraph()
		{}

		const NodeVector& getNodes() const { return _nodes; }

		const EdgeList& getEdges(int idx) const;

		//returns the node at the given index
		const Node& getNode(int idx)const;

		//non const version
		Node& getNode(int idx);

		//const method for obtaining a reference to an edge
		const Edge& getEdge(int from, int to)const;

		//non const version
		Edge& getEdge(int from, int to);

		//adds a node to the graph and returns its index
		int addNode(Node node);

		//Use this to add an edge to the graph. The method will ensure that the
		//edge passed as a parameter is valid before adding it to the graph. If the
		//graph is a digraph then a similar edge connecting the nodes in the opposite
		//direction will be automatically added.
		void addEdge(Edge edge);

		int numNodes() const { return static_cast<int>(_nodes.size()); }

		//sets the cost of an edge
		void setEdgeCost(int from, int to, double cost);

		//returns true if the graph contains no nodes
		bool isEmpty()const { return _nodes.empty(); }

		//clears the graph ready for new node insertions
		void clear() { _next_node_index = 0; _nodes.clear(); _edges.clear(); }

		//retrieves the next free node index
		int getNextFreeNodeIndex()const { return _next_node_index; }

	private:
		//the nodes that comprise this graph
		NodeVector			_nodes;

		//a vector of adjacency edge lists. (each node index keys into the 
		//list of edges associated with that node)
		EdgeListVector		_edges;

		int					_next_node_index;

	};

	template <class Node, class Edge>
	const typename SparseGraph<Node, Edge>::EdgeList& SparseGraph<Node, Edge>::getEdges(int idx) const
	{
		assert((idx < (int)_nodes.size()) && (idx >= 0) &&
			"<SparseGraph::getNode>: invalid index");
		return _edges[idx];
	}

	//------------------------------ getNode -------------------------------------
	//
	//  const and non const methods for obtaining a reference to a specific node
	//----------------------------------------------------------------------------
	template <class Node, class Edge>
	const Node&  SparseGraph<Node, Edge>::getNode(int idx)const
	{
		assert((idx < (int)_nodes.size()) && (idx >= 0) &&
			"<SparseGraph::getNode>: invalid index");

		return _nodes[idx];
	}

	//non const version
	template <class Node, class Edge>
	Node&  SparseGraph<Node, Edge>::getNode(int idx)
	{
		assert((idx < (int)_nodes.size()) && (idx >= 0) &&
			"<SparseGraph::getNode>: invalid index");

		return _nodes[idx];
	}

	//------------------------------ getEdge -------------------------------------
	//
	//  const and non const methods for obtaining a reference to a specific edge
	//----------------------------------------------------------------------------
	template <class Node, class Edge>
	const Edge& SparseGraph<Node, Edge>::getEdge(int from, int to)const
	{
		assert((from < (int)_nodes.size()) && (from >= 0) &&
			_nodes[from].getIndex() != -1 &&
			"<SparseGraph::getEdge>: invalid 'from' index");

		assert((to < (int)_nodes.size()) && (to >= 0) &&
			_nodes[to].getIndex() != -1 &&
			"<SparseGraph::getEdge>: invalid 'to' index");

		for (typename EdgeList::const_iterator eg = _edges[from].begin(); eg != _edges[from].end(); ++eg)
		{
			if (eg->getTo() == to) return *eg;
		}

		assert(0 && "<SparseGraph::getEdge>: edge does not exist");
	}

	//non const version
	template <class Node, class Edge>
	Edge& SparseGraph<Node, Edge>::getEdge(int from, int to)
	{
		assert((from < (int)_nodes.size()) && (from >= 0) &&
			_nodes[from].getIndex() != -1 &&
			"<SparseGraph::getEdge>: invalid 'from' index");

		assert((to < (int)_nodes.size()) && (to >= 0) &&
			_nodes[to].getIndex() != -1 &&
			"<SparseGraph::getEdge>: invalid 'to' index");

		for (typename EdgeList::iterator eg = _edges[from].begin(); eg != _edges[from].end(); ++eg)
		{
			if (eg->getTo() == to) return *eg;
		}

		assert(0 && "<SparseGraph::getEdge>: edge does not exist");
	}

	//-------------------------- addEdge ------------------------------------------
	//
	//  Use this to add an edge to the graph. The method will ensure that the
	//  edge passed as a parameter is valid before adding it to the graph. If the
	//  graph is a digraph then a similar edge connecting the nodes in the opposite
	//  direction will be automatically added.
	//-----------------------------------------------------------------------------
	template <class Node, class Edge>
	void SparseGraph<Node, Edge>::addEdge(Edge edge)
	{
		//first make sure the from and to nodes exist within the graph 
		assert((0 <= edge.getFrom() && edge.getFrom() < (int)_nodes.size()) &&
			(0 <= edge.getTo() && edge.getTo() < (int)_nodes.size()) &&
			"<SparseGraph::addEdge>: invalid node index");

		_edges[edge.getFrom()].push_back(edge);
		_edges[edge.getTo()].push_back(Edge(edge.getTo(), edge.getFrom(), edge.getCost()));
	}



	//-------------------------- addNode -------------------------------------
	//
	//  Given a node this method first checks to see if the node has been added
	//  previously but is now innactive. If it is, it is reactivated.
	//
	//  If the node has not been added previously, it is checked to make sure its
	//  index matches the next node index before being added to the graph
	//------------------------------------------------------------------------
	template <class Node, class Edge>
	int SparseGraph<Node, Edge>::addNode(Node node)
	{
		//make sure the new node has been indexed correctly
		assert(node.getIndex() == _next_node_index &&
			"<SparseGraph::addNode>:invalid index");

		_nodes.push_back(node);
		_edges.push_back(EdgeList());

		return _next_node_index++;
	}



	//-------------------------- setEdgeCost ---------------------------------
	//
	//  Sets the cost of a specific edge
	//------------------------------------------------------------------------
	template <class Node, class Edge>
	void SparseGraph<Node, Edge>::setEdgeCost(int from, int to, double NewCost)
	{
		//make sure the nodes given are valid
		assert((from < (int)_nodes.size()) && (to < (int)_nodes.size()) &&
			"<SparseGraph::setEdgeCost>: invalid index");

		//visit each neighbour and erase any edges leading to this node
		for (typename EdgeList::iterator eg = _edges[from].begin(); eg != _edges[from].end(); ++eg)
		{
			if (eg->getTo() == to)
			{
				eg->setCost(NewCost);
				break;
			}
		}
	}
}

#endif
