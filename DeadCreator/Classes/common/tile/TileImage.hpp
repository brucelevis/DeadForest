//
//  TileImage.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 6. 29..
//
//

#pragma once

#include "cocos2d.h"

namespace realtrick
{
    
    class TileImage : public cocos2d::Sprite
    {
        
    public:
        
        TileImage() = default;
        virtual ~TileImage() = default;
        static TileImage* create()
        {
            auto ret = new (std::nothrow) TileImage();
            if ( ret && ret->init() )
            {
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }
        
        static TileImage* create(const std::string& fileName)
        {
            auto ret = new (std::nothrow) TileImage();
            if ( ret && ret->initWithFile(fileName) )
            {
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }
        
        static TileImage* createWithFrameName(const std::string& frameName)
        {
            auto ret = new (std::nothrow) TileImage();
            if ( ret && ret->initWithSpriteFrameName(frameName) )
            {
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }
        
    };
    
}









