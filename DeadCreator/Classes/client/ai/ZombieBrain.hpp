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
            
			static ZombieBrain* createDefault(HumanBase* owner)
			{
				ZombieBrain* brain = new ZombieBrain(owner);
				GoalAttackTarget* goal = new GoalAttackTarget(owner);
				goal->setEvaluator([brain](HumanBase* owner) { return 1; });
				brain->getGoalThink()->addGoalEntry(goal);
				return brain;
			}

			static ZombieBrain* createWithDestination(
				HumanBase* owner,
				const cocos2d::Vec2& desti,
				float arriveRange = 100)
			{
				ZombieBrain* brain = new ZombieBrain(owner);
				GoalAttackToDestination* goal =
					new GoalAttackToDestination(owner, desti, arriveRange);
				goal->setEvaluator([brain](HumanBase* owner) { return 1; });
				brain->getGoalThink()->addGoalEntry(goal);
				return brain;
			}

            explicit ZombieBrain(HumanBase* owner) : BrainBase(owner)
            {
                _thinker = new GoalThink(owner);
            }
            
			GoalThink* getGoalThink() const { return _thinker; }

            virtual ~ZombieBrain()
            {
                delete _thinker;
                _thinker = nullptr;
            }
            
            virtual void think() override
            {
				if(_regulator.isReady())
					_thinker->process();
            }
            
        private:
            
            GoalThink* _thinker;
            
        };
        
    }
}









