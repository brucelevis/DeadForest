//
//  CrossHair.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 11..
//
//

#include "CrossHair.hpp"
using namespace realtrick::client;
using namespace cocos2d;

CrossHair* CrossHair::create(const std::string& baseImage)
{
    auto ret = new (std::nothrow) CrossHair();
    if ( ret && ret->init(baseImage) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool CrossHair::init(const std::string& baseImage)
{
    if ( !Node::init() )
        return false;
    
    _baseImage = Sprite::create(baseImage);
    addChild(_baseImage);
    
    return true;
}


void CrossHair::setRange(float range)
{
    _baseImage->setPosition(Vec2(range, 0.0f));
}









