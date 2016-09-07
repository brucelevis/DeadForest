//
//  BrainBase.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 10..
//
//

#pragma once
#include "Regulator.h"
#include <memory>

namespace realtrick
{
    namespace client
    {
        
        class HumanBase;
        
        class BrainBase
        {
            
        public:
            
            explicit BrainBase(HumanBase* owner, Regulator* regulator) 
				: _owner(owner), _regulator(regulator) {}
            virtual ~BrainBase() = default;
            
            virtual void think() = 0;
            
        protected:
            
            HumanBase* _owner;
			std::unique_ptr<Regulator> _regulator;
			
            
        };
        
    }
}









