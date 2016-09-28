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
        
        class GoalSeekToPosition : public GoalBase
        {
            
        public:
			
			GoalSeekToPosition(HumanBase* owner, cocos2d::Vec2 target);
			virtual ~GoalSeekToPosition() override
			{}

            virtual void activate() override;
            virtual GoalStatus process() override;
            virtual void terminate() override;

        private:
        
			//returns true if the bot gets stuck
			bool		isStuck() const;

			//the position the bot is moving to
			cocos2d::Vec2	_position;

			std::chrono::duration<double>	_start;

			//the estimated time the bot should take to traverse the edge
			std::chrono::duration<double>	_time_expected;
        };
        
    }
}







