//
//  SoundSource.hpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 3. 6..
//
//

#pragma once

#include <cocos2d.h>
#include <string>

namespace realtrick
{
    
    class SoundSource
    {
    public:
        std::string fileName;
        cocos2d::Vec2 position;
        float soundRange = 300.0f;
        float volume = 1.0f;
    };

}

