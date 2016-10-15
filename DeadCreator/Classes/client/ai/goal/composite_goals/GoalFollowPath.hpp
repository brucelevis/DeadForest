//
//  GoalTraverseEdge.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include <chrono>

#include "GoalCompositeBase.hpp"
#include "PathEdge.h"

namespace realtrick
{
    namespace client
    {
        class GoalFollowPath : public GoalCompositeBase
        {
            
        public:
			GoalFollowPath(
				HumanBase* const bot,
				std::list<realtrick::PathEdge> path);

			virtual ~GoalFollowPath() override
			{}

            virtual void activate() override;
            virtual GoalStatus process() override;
            virtual void terminate() override;

        private:
        
			//a local copy of the path returned by the path planner
			std::list<realtrick::PathEdge>  _path;

			
        };
    }
}



