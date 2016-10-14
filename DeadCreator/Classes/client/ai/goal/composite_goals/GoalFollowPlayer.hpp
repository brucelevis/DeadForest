//
//  GoalFollowPlayer.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include <chrono>

#include "GoalCompositeBase.hpp"

namespace realtrick
{
    namespace client
    {
        class GoalFollowPlayer : public GoalCompositeBase
        {
            
        public:
			explicit GoalFollowPlayer(HumanBase* const owner);

			virtual ~GoalFollowPlayer() override
			{}

			int evaluate(HumanBase* const owner);

            virtual void activate() override;
            virtual GoalStatus process() override;
			virtual void terminate() override;
        };
    }
}


