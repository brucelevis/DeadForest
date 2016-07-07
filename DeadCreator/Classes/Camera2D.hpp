//
//  Camera2D.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 29..
//
//

#pragma once

#include "cocos2d.h"

namespace realtrick
{
    
    class Camera2D : public cocos2d::Node
    {
        
    public:
        
        Camera2D() = default;
        
        virtual ~Camera2D() = default;
        
        CREATE_FUNC(Camera2D);
        
    };
    
}

