//
//  TileImage.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 29..
//
//

#include "TileImage.hpp"
#include "GMXLayer2.hpp"
using namespace realtrick;
using namespace cocos2d;

TileImage::TileImage()
{
}


TileImage::~TileImage()
{
}



TileImage* TileImage::create()
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


TileImage* TileImage::create(const std::string& fileName)
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


TileImage* TileImage::createWithFrameName(const std::string& frameName)
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




