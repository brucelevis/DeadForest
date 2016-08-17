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
                
                GoalHuntTarget* huntTarget = new GoalHuntTarget(_owner);
                huntTarget->setEvaluator([this](HumanBase* owner) { return 1; });
                
                _thinker->addGoalEntry(huntTarget);
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









