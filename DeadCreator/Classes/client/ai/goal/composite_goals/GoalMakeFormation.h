#ifndef GOAL_MAKE_FORMATION_H
#define GOAL_MAKE_FORMATION_H
#pragma warning (disable:4786)

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



#endif

