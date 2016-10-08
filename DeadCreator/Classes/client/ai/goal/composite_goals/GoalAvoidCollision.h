#ifndef GOAL_AVOID_COLLISION_H
#define GOAL_AVOID_COLLISION_H
#pragma warning (disable:4786)

#include "GoalCompositeBase.hpp"
#include "Types.hpp"

namespace realtrick
{
	namespace client
	{
		class HumanBase;

		class GoalAvoidCollision : public GoalCompositeBase
		{

		public:

			GoalAvoidCollision(HumanBase* owner);
			virtual ~GoalAvoidCollision() override
			{}

			int evaluate(HumanBase* const owner);

			virtual void activate() override;
			virtual GoalStatus process() override;
			virtual void terminate() override;

		private:

			std::chrono::duration<double> _startTime;
		};
	}
}



#endif

