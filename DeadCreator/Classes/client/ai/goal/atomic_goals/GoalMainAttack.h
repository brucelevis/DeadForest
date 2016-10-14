#ifndef GOAL_MAIN_ATTACK_H
#define GOAL_MAIN_ATTACK_H
#pragma warning (disable:4786)

#include "GoalBase.hpp"

namespace realtrick
{
	namespace client
	{
		class HumanBase;

		class GoalMainAttack : public GoalBase
		{

		public:
			static cocos2d::Vec2 smartMoving(HumanBase* owner);
			static cocos2d::Vec2 roughMoving(HumanBase* owner);

			GoalMainAttack(
				HumanBase* owner,
				const cocos2d::Vec2& target,
				std::function<cocos2d::Vec2(HumanBase*)> movingStrategy);

			virtual ~GoalMainAttack() override
			{}

			virtual void activate() override;
			virtual GoalStatus process() override;
			virtual void terminate() override;

		public:

			cocos2d::Vec2 _target;
			std::chrono::duration<double>	_startTime;
			std::function<cocos2d::Vec2(HumanBase*)> _movingStrategy;
		};
	}
}



#endif

