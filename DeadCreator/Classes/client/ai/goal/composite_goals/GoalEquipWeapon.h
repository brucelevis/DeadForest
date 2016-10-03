#ifndef GOAL_EQUIP_WEAPON_H
#define GOAL_EQUIP_WEAPON_H
#pragma warning (disable:4786)

#include "GoalCompositeBase.hpp"
#include "Types.hpp"

namespace realtrick
{
	namespace client
	{
		class HumanBase;

		class GoalEquipWeapon : public GoalCompositeBase
		{

		public:

			GoalEquipWeapon(HumanBase* owner);
			virtual ~GoalEquipWeapon() override
			{}

			int evaluate(HumanBase* const owner);
			void makeEquipItemWeight();
			EntityType getBestItem(float& weight) const;

			virtual void activate() override;
			virtual GoalStatus process() override;
			virtual void terminate() override;

		private:
			std::chrono::duration<double> _startTime;
			std::map<EntityType, float> _weightEquipItem;
			EntityType _weaponType;
		};
	}
}



#endif

