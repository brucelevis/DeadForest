//
//  Sheriff.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "Sheriff.hpp"
using namespace cocos2d;

Sheriff::Sheriff(GMXLayer2& layer, int id) :
EntityBase(layer, id)
{
    _type = EntityType::SHERIFF;
}


Sheriff::~Sheriff()
{
}


Sheriff* Sheriff::create(GMXLayer2& layer, int id, cocos2d::ui::Widget::TextureResType resType)
{
    auto ret = new (std::nothrow) Sheriff(layer, id);
    if ( ret && ret->init(resType) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool Sheriff::init(cocos2d::ui::Widget::TextureResType resType)
{
    if ( resType == cocos2d::ui::Widget::TextureResType::LOCAL )
    {
        _body = Sprite::create("HumanFistIdleLoop0.png");
        _shadow = Sprite::create("HumanFistIdleLoop0.png");
    }
    else if ( resType == cocos2d::ui::Widget::TextureResType::PLIST )
    {
        _body = Sprite::createWithSpriteFrameName("HumanFistIdleLoop0.png");
        _shadow = Sprite::createWithSpriteFrameName("HumanFistIdleLoop0.png");
    }
    
    _shadow->setColor(Color3B::BLACK);
    _shadow->setPosition(Vec2(10, -5));
    
    addChild(_shadow);
    addChild(_body);
    
    
    
    return true;
}