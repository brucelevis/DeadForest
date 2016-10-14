#ifndef GOAL_RELOAD_H
#define GOAL_RELOAD_H
#pragma warning (disable:4786)

#include "GoalEvaluatable.hpp"
#include "Types.hpp"

namespace realtrick
{
	namespace client
	{
		class HumanBase;

		class GoalReload : public GoalEvaluatable
		{

		public:

			GoalReload(HumanBase* owner, float character_bias = 0.0f);
			virtual ~GoalReload() override
			{}

			virtual int evaluate(HumanBase* const owner) override;
			virtual void activate() override;
			virtual GoalStatus process() override;
			virtual void terminate() override;

		private:
			std::chrono::duration<double> _startTime;
		};
	}
}



#endif

