#pragma once

#include "GoalCompositeBase.hpp"
#include "Types.hpp"

namespace realtrick
{
	namespace client
	{
		class HumanBase;

		class GoalAvoid : public GoalCompositeBase
		{

		public:

			GoalAvoid(HumanBase* owner);
			virtual ~GoalAvoid() override
			{}

			virtual void activate() override;
			virtual GoalStatus process() override;
			virtual void terminate() override;
            
		};
	}
}









