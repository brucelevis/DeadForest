#ifndef GOAL_RELOAD_H
#define GOAL_RELOAD_H
#pragma warning (disable:4786)

#include "GoalCompositeBase.hpp"
#include "Types.hpp"

namespace realtrick
{
	namespace client
	{
		class HumanBase;

		class GoalReload : public GoalCompositeBase
		{

		public:

			GoalReload(HumanBase* owner);
			virtual ~GoalReload() override
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

