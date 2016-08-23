#ifndef TERMINATION_POLICIES_H
#define TERMINATION_POLICIES_H
//-----------------------------------------------------------------------------
//
//  Name:   SearchTerminationPolicies.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   class templates to define termination policies for Dijkstra's
//          algorithm
//-----------------------------------------------------------------------------


namespace realtrick
{

	//--------------------------- FindNodeIndex -----------------------------------
	//the search will terminate when the currently examined graph node
	//is the same as the target node.
	//-----------------------------------------------------------------------------

	class FindNodeIndex
	{
	public:

		template <class Graph>
		static bool isSatisfied(const Graph& graph, int target, int current_node_idx)
		{
			return current_node_idx == target;
		}
	};


}
#endif