//
//  BrainBase.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 10..
//
//

#pragma once


namespace realtrick
{
    namespace client
    {
        
        class HumanBase;
        
        class BrainBase
        {
            
        public:
            
            explicit BrainBase(HumanBase* owner) : _owner(owner) {}
            virtual ~BrainBase() = default;
            
            virtual void think() = 0;
            
        protected:
            
            HumanBase* _owner;
            
        };
        
    }
}









