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
        class GoalMoveToPosition : public GoalCompositeBase
        {
            
        public:
			GoalMoveToPosition(
				HumanBase* const owner,
				cocos2d::Vec2 pos);

			virtual ~GoalMoveToPosition() override
			{}

            virtual void activate() override;
            virtual GoalStatus process() override;
			virtual void terminate() override;

        private:
        
			//the position the bot wants to reach
			cocos2d::Vec2 _destination;
        };
    }
}


