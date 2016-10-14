//
//  GoalFollowPlayer.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include <chrono>
#include "GoalEvaluatable.hpp"


namespace realtrick
{
    namespace client
    {
        class GoalFollowPlayer : public GoalEvaluatable
        {
            
        public:
			explicit GoalFollowPlayer(HumanBase* const owner, float character_bias = 0.0f);

			virtual ~GoalFollowPlayer() override
			{}

			virtual int evaluate(HumanBase* const owner) override;
            virtual void activate() override;
            virtual GoalStatus process() override;
			virtual void terminate() override;
        };
    }
}


