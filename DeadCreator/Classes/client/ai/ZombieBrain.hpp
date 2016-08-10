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
            
            explicit ZombieBrain(HumanBase* owner) : BrainBase(owner) {}
            virtual ~ZombieBrain() = default;
            
            virtual void think() override
            {
                _thinker->process();
            }
            
        private:
            
            GoalThink* _thinker;
            
        };
        
    }
}









