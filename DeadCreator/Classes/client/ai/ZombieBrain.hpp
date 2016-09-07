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

namespace realtrick
{
    namespace client
    {
        
        class ZombieBrain : public BrainBase
        {
            
        public:
            
            explicit ZombieBrain(HumanBase* owner) : BrainBase(owner, new Regulator(5))
            {
                _thinker = new GoalThink(owner);
                
                /*GoalWander* wander = new GoalWander(_owner);
                wander->setEvaluator([this](HumanBase* owner) { return 1; });
                
                _thinker->addGoalEntry(wander);*/

				/*GoalSeekToPosition* seek = new GoalSeekToPosition(owner, cocos2d::Vec2(1000, 1000));
				seek->setEvaluator([this](HumanBase* owner) { return 1; });

				_thinker->addGoalEntry(seek);*/

				/*GoalTraverseEdge* traverse = new GoalTraverseEdge(owner, PathEdge(cocos2d::Vec2(500, 500), cocos2d::Vec2(1000, 1000)), true);
				traverse->setEvaluator([this](HumanBase* owner) { return 1; });

				_thinker->addGoalEntry(traverse);*/

				/*GoalMoveToPosition* move = 
					new GoalMoveToPosition(owner, cocos2d::Vec2(2400, 1900));
				move->setEvaluator([this](HumanBase* owner) { return 1; });
				_thinker->addGoalEntry(move);
				*/

				GoalAttackTarget* attack = new GoalAttackTarget(owner);
				attack->setEvaluator([this](HumanBase* owner) { return 1; });
				_thinker->addGoalEntry(attack);

            }
            
            virtual ~ZombieBrain()
            {
                delete _thinker;
                _thinker = nullptr;
            }
            
            virtual void think() override
            {
				if(_regulator->isReady())
					_thinker->process();
            }
            
        private:
            
            GoalThink* _thinker;
            
        };
        
    }
}









