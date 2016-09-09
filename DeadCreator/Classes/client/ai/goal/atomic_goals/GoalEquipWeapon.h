#ifndef GOAL_EQUIP_WEAPON_H
#define GOAL_EQUIP_WEAPON_H
#pragma warning (disable:4786)

#include "GoalBase.hpp"
#include "Types.hpp"

namespace realtrick
{
	namespace client
	{
		class HumanBase;

		class GoalEquipWeapon : public GoalBase
		{

		public:

			GoalEquipWeapon(HumanBase* owner, EntityType weaponType);
			virtual ~GoalEquipWeapon() override
			{}

			virtual void activate() override;
			virtual GoalStatus process() override;
			virtual void terminate() override;

		public:
			std::chrono::duration<double>	_startTime;
			EntityType _weaponType;
		};
	}
}



#endif

