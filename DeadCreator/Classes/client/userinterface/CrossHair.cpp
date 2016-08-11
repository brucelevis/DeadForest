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
    
    _baseImageName = baseImage;
    
    return true;
}


void CrossHair::setRange(float range)
{
    _baseImage->setPosition(Vec2(range, 0.0f));
}


void CrossHair::setEffect(const std::string& name, const CrossHairEffect& effect)
{
    _effects.insert( {name, effect} );
}


void CrossHair::runEffect(const std::string& name)
{
    if ( _effects.count(name) != 0 )
    {
        auto& effect = _effects.at(name);
        auto animation = Animation::create();
        animation->setDelayPerUnit(effect.frameSwapTime);
        for ( const auto& frame : effect.frames ) animation->addSpriteFrameWithFile(frame);
        
        _baseImage->stopAllActions();
        _baseImage->runAction(Sequence::create(Spawn::create(Animate::create(animation),
                                                             Sequence::create(ScaleTo::create(effect.frameSwapTime / 4 * effect.frames.size(), 0.5f),
                                                                              ScaleTo::create(effect.frameSwapTime / 2 * effect.frames.size(), 1.5f),
                                                                              ScaleTo::create(effect.frameSwapTime / 4 * effect.frames.size(), 1.0f),
                                                                              nullptr),
                                                             nullptr),
                                               CallFunc::create([this] { _baseImage->setTexture(_baseImageName); }),
                                               nullptr));
    }
}









