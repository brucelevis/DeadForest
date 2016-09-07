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
        class GoalAttackTarget : public GoalCompositeBase
        {
            
        public:
			explicit GoalAttackTarget(HumanBase* const owner);

			virtual ~GoalAttackTarget() override
			{}

            virtual void activate() override;
            virtual GoalStatus process() override;
			virtual void terminate() override
			{}
        };
    }
}


