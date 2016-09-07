//
//  AnimationBase.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 14..
//
//

#pragma once

#include <vector>
#include <deque>

#include "cocos2d.h"

namespace realtrick
{
    namespace client
    {
        
        class AnimationBase
        {
            
        public:
            
            virtual ~AnimationBase() = default;
            
            int getStartFrame() const                   { return _startFrame; }
            int getMaxFrame() const                     { return _maxFrame; }
            int getPriority() const                     { return _priority; }
            std::string getFileName() const             { return _fileName; }
            float getFrameSwapTime() const              { return _frameSwapTime; }
            bool isReversePlay() const                  { return _isReversePlay; }

        protected:
            
            int                 _startFrame;
            int                 _maxFrame;
            int                 _priority;
            std::string         _fileName;
            float               _frameSwapTime;
            bool                _isReversePlay;
            
        };
        
    }
}













