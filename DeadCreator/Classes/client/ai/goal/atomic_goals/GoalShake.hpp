#ifndef GOAL_SHAKE_H
#define GOAL_SHAKE_H
#pragma warning (disable:4786)

#include "GoalBase.hpp"

namespace realtrick
{
	namespace client
	{
		class HumanBase;

		class GoalShake : public GoalBase
		{

		public:

			GoalShake(
				HumanBase* owner,
				const cocos2d::Vec2& movingDirection,
				const cocos2d::Vec2& shakingAxis,
				float shakeTime);

			virtual ~GoalShake() override
			{}

			virtual void activate() override;
			virtual GoalStatus process() override;
			virtual void terminate() override;

		public:
			std::chrono::duration<double> _startTime;
			cocos2d::Vec2 _movingDirection;
			cocos2d::Vec2 _shakingAxis;
			float _shakeTime;
		};
	}
}



#endif

