#pragma once

#include "GoalCompositeBase.hpp"
#include "Types.hpp"

namespace realtrick
{
	namespace client
	{
		class HumanBase;

		class GoalMakeFormation : public GoalCompositeBase
		{

		public:

			GoalMakeFormation(HumanBase* owner);
			virtual ~GoalMakeFormation() override
			{}

			virtual void activate() override;
			virtual GoalStatus process() override;
			virtual void terminate() override;

		};
	}
}









