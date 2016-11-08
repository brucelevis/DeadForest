//
//  PursuerBrain.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include "BrainBase.hpp"
#include "Goals.hpp"
#include "Regulator.h"

namespace realtrick
{
    namespace client
    {
		class GoalAttackTarget;
        class PursuerBrain : public BrainBase
        {
            
        public:
			static PursuerBrain* createDefault(HumanBase* owner);

			virtual ~PursuerBrain()
			{}

		private:
			explicit PursuerBrain(HumanBase* owner)	: BrainBase(owner)
			{}
        };
    }
}









