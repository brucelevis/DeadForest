//
//  CrossHair.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 11..
//
//

#pragma once

#include "cocos2d.h"

namespace realtrick
{
    namespace client
    {
        
        class CrossHair : public cocos2d::Node
        {
            
        public:
            
            CrossHair() = default;
            virtual ~CrossHair() = default;
            
            static CrossHair* create(const std::string& baseImage);
            bool init(const std::string& baseImage);
            void setRange(float range);
            
        private:
            
            cocos2d::Sprite* _baseImage;
            
        };
        
    }
}









