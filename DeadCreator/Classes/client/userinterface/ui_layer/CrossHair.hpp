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
        
        struct CrossHairEffect
        {
            std::vector<std::string> frames;
            float frameSwapTime;
            
            CrossHairEffect(const std::vector<std::string>& frames, float frameSwapTime) :
            frames(frames),
            frameSwapTime(frameSwapTime)
            {}
        };
        
        class CrossHair : public cocos2d::Node
        {
            
        public:
            
            CrossHair() = default;
            virtual ~CrossHair() = default;
            
            static CrossHair* create(const std::string& baseImage);
            bool init(const std::string& baseImage);
            void setRange(float range);
            void setEffect(const std::string& name, const CrossHairEffect& effect);
            void runEffect(const std::string& name);
            
        private:
            
            cocos2d::Sprite* _baseImage;
            std::string _baseImageName;
            std::map<std::string, CrossHairEffect> _effects;
            
        };
        
    }
}









