//
//  InputCommandBase.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 4..
//
//

#pragma once

#include "HumanBase.hpp"

namespace realtrick
{
    namespace client
    {
        
        class InputCommandBase
        {
          
        public:
            
            virtual ~InputCommandBase() = default;
            virtual void execute() = 0;
            
        };
        
    }
}