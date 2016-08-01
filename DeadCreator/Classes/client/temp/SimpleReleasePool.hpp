//
//  SimpleReleasePool.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 1..
//
//

#pragma once

#include "cocos2d.h"

namespace realtrick
{
    namespace client
    {
        
        class SimpleReleasePool
        {
            
        public:
            
            virtual ~SimpleReleasePool()
            {
                for( auto iter = std::begin(_releasePool); iter != std::end(_releasePool); ++iter )
                    (*iter)->release();
                _releasePool.clear();
            }
            
            void addObject(cocos2d::Ref* object) { object->retain(); _releasePool.push_back(object); }
          
        private:
            
            std::vector<cocos2d::Ref*> _releasePool;
            
        };
        
    }
}
