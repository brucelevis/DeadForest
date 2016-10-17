//
//  ZombieBrain.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 10..
//
//

#include "BrainBase.hpp"
#include "Goals.hpp"
#include "GoalMoveToPosition.hpp"
#include "GoalAttackTarget.hpp"
#include "GoalAttackToDestination.hpp"

namespace realtrick
{
    namespace client
    {
        class ZombieBrain : public BrainBase
        {  
        public:
            
			static ZombieBrain* createDefault(HumanBase* owner);

			static ZombieBrain* createWithDestination(
				HumanBase* owner,
				const cocos2d::Vec2& desti,
				float arriveRange = 100);

			virtual void think() override;


		private:

			explicit ZombieBrain(HumanBase* owner) : BrainBase(owner)
			{}
        };
    }
}









