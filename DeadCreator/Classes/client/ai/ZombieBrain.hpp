//
//  ZombieBrain.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 10..
//
//

#include "BrainBase.hpp"
#include "Goals.hpp"

namespace realtrick
{
    namespace client
    {
        
        class ZombieBrain : public BrainBase
        {
            
        public:
            
            explicit ZombieBrain(HumanBase* owner) : BrainBase(owner)
            {
                _thinker = new GoalThink(owner);
                
                GoalWander* wander = new GoalWander(_owner);
                wander->setEvaluator([this](HumanBase* owner) { return 1; });
                
                _thinker->addGoalEntry(wander);
            }
            
            virtual ~ZombieBrain()
            {
                delete _thinker;
                _thinker = nullptr;
            }
            
            virtual void think() override
            {
                _thinker->process();
            }
            
        private:
            
            GoalThink* _thinker;
            
        };
        
    }
}









