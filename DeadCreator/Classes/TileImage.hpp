//
//  TileImage.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 29..
//
//

#pragma once

#include "cocos2d.h"

namespace realtrick
{
    
    class TileImage : public cocos2d::Sprite
    {
        
    public:
        
        TileImage();
        
        virtual ~TileImage();
        
        static TileImage* create();
        
        static TileImage* create(const std::string& fileName);
        
        static TileImage* createWithFrameName(const std::string& frameName);
        
    private:
        
    };
    
}
