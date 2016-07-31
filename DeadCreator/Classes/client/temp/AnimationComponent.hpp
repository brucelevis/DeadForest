//
//  AnimationComponent.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 31..
//
//

#pragma once

#include "ComponentBase.hpp"


namespace realtrick
{
    namespace client
    {
        
        class AnimationComponent : public ComponentBase
        {
            
        public:
            
            virtual void update(float dt) override;
            
        private:
            
            
            
        };
        
    }
}