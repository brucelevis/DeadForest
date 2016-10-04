#ifndef GOAL_RUN_AWAY_H
#define GOAL_RUN_AWAY_H
#pragma warning (disable:4786)

#include "GoalCompositeBase.hpp"
#include "Types.hpp"

namespace realtrick
{
	namespace client
	{
		class HumanBase;

		class GoalRunAway : public GoalCompositeBase
		{

		public:

			GoalRunAway(HumanBase* owner);
			virtual ~GoalRunAway() override
			{}

			int evaluate(HumanBase* const owner);

			virtual void activate() override;
			virtual GoalStatus process() override;
			virtual void terminate() override;

		public:
		};
	}
}



#endif

