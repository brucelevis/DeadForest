//
//  GoalAttackTarget.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 8. 17..
//
//

#include <chrono>

#include "GoalEvaluatable.hpp"
#include "PathEdge.h"

namespace realtrick
{
    namespace client
    {
        class GoalAttackTarget : public GoalEvaluatable
        {
        public:
			explicit GoalAttackTarget(HumanBase* const owner, float character_bias = 1.0f);

			virtual ~GoalAttackTarget() override
			{}

			virtual int evaluate(HumanBase* const owner) override;
            virtual void activate() override;
            virtual GoalStatus process() override;
			virtual void terminate() override
			{}
        };
    }
}


