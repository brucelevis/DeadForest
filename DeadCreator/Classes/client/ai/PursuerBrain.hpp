//
//  PursuerBrain.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include "BrainBase.hpp"
#include "Goals.hpp"

namespace realtrick
{
    namespace client
    {
        
        class PursuerBrain : public BrainBase
        {
            
        public:
            
            explicit PursuerBrain(HumanBase* owner) : BrainBase(owner)
            {
                _thinker = new GoalThink(owner);
//                
//                GoalWander* wander = new GoalWander(_owner);
//                wander->setEvaluator([this](HumanBase* owner) { return 1; });
//                
//                _thinker->addGoalEntry(wander);
            }
            
            virtual ~PursuerBrain()
            {
                delete _thinker;
                _thinker = nullptr;
            }
            
            virtual void think() override
            {
                if ( _owner->isAlive() ) _thinker->process();
            }
            
        private:
            
            GoalThink* _thinker;
            
        };
        
    }
}









