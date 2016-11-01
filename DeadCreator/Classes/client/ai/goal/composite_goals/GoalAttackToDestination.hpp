//
//  GoalAttackToDestination.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 8. 17..
//
//

#include <chrono>

#include "GoalCompositeBase.hpp"
#include "PathEdge.h"

namespace realtrick
{
    namespace client
    {
        class GoalAttackToDestination : public GoalCompositeBase
        {
            
        public:
			explicit GoalAttackToDestination(
				HumanBase* const owner,
				const cocos2d::Vec2& destination,
				float arriveRange);

			virtual ~GoalAttackToDestination() override
			{}

            virtual void activate() override;
            virtual GoalStatus process() override;
			virtual void terminate() override;

		private:
			cocos2d::Vec2 _destination;
			float _arriveRange;
        };
    }
}


