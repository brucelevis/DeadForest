#ifndef GOAL_AVOID_H
#define GOAL_AVOID_H
#pragma warning (disable:4786)

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



#endif

