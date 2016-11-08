//
//  GoalRaid.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include <chrono>

#include "GoalCompositeBase.hpp"
#include "PathEdge.hpp"

namespace realtrick
{
    namespace client
    {
        class GoalRaid : public GoalCompositeBase
        {
        public:
			explicit GoalRaid(
				HumanBase* const owner,
				std::function<cocos2d::Vec2(HumanBase*)> movingStrategy);

			virtual ~GoalRaid() override
			{}

            virtual void activate() override;
            virtual GoalStatus process() override;
			virtual void terminate() override;

		private:
			bool _arrived;
			std::function<cocos2d::Vec2(HumanBase*)> _movingStrategy;
        };
    }
}


