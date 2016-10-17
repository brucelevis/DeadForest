//
//  GoalTraverseEdge.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include <chrono>

#include "GoalBase.hpp"
#include "PathEdge.h"

namespace realtrick
{
    namespace client
    {
        class GoalTraverseEdge : public GoalBase
        {
            
        public:
			
            GoalTraverseEdge(HumanBase* owner, realtrick::PathEdge edge, bool last_edge);
			virtual ~GoalTraverseEdge()
			{}
            
            virtual void activate() override;
            virtual GoalStatus process() override;
            virtual void terminate() override;
            
			realtrick::PathEdge getEdge() const;

        private:
        
			//returns true if the bot gets stuck
			bool		isStuck() const;

			//the edge the bot will follow
			realtrick::PathEdge	_edge;

			//true if _edge is the last in the path.
			bool		_last_edge_in_path;

			std::chrono::duration<double>	_start;

			//the estimated time the bot should take to traverse the edge
			std::chrono::duration<double>	_time_expected;
        };
        
    }
}







