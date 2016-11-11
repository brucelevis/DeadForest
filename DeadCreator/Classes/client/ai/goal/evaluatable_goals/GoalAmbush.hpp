#pragma once

#include "GoalEvaluatable.hpp"
#include "Types.hpp"

namespace realtrick
{
	namespace client
	{
		class HumanBase;

		class GoalAmbush : public GoalEvaluatable
		{

		public:

			GoalAmbush(HumanBase* owner, float character_bias = 0.0f);
			virtual ~GoalAmbush() override
			{}

			virtual int evaluate(HumanBase* const owner) override;
			virtual void activate() override;
			virtual GoalStatus process() override;
			virtual void terminate() override;
            
		};
	}
}









