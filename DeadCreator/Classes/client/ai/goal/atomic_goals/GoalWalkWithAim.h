#ifndef GOAL_WALK_WITH_AIM_H
#define GOAL_WALK_WITH_AIM_H
#pragma warning (disable:4786)

#include "GoalBase.hpp"

namespace realtrick
{
	namespace client
	{
		class HumanBase;

		class GoalWalkWithAim : public GoalBase
		{

		public:

			GoalWalkWithAim(
				HumanBase* owner,
				const cocos2d::Vec2& destination,
				const cocos2d::Vec2& aimHeading,
				float walkTime);

			virtual ~GoalWalkWithAim() override
			{}

			virtual void activate() override;
			virtual GoalStatus process() override;
			virtual void terminate() override;

		public:
			std::chrono::duration<double> _startTime;
			cocos2d::Vec2 _destination;
			cocos2d::Vec2 _aimHeading;
			float _walkTime;
		};
	}
}



#endif

