//
//  GoalTraverseEdge.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#pragma once


#include <chrono>

#include "GoalBase.hpp"
#include "PathEdge.hpp"

namespace realtrick
{
    namespace client
    {
        class GoalTraverseEdge : public GoalBase
        {
            
        public:
			
            GoalTraverseEdge(HumanBase* owner, realtrick::PathEdge edge);
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

			std::chrono::duration<double>	_start;

			//the estimated time the bot should take to traverse the edge
			std::chrono::duration<double>	_time_expected;
        };
        
    }
}









