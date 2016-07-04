//
//  Item9mm.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "Item9mm.hpp"
using namespace cocos2d;

Item9mm::Item9mm(GMXLayer2& layer, int id) :
EntityBase(layer, id)
{
    _type = EntityType::ITEM_9MM;
}


Item9mm::~Item9mm()
{
}


Item9mm* Item9mm::create(GMXLayer2& layer, int id, const std::string& fileName, cocos2d::ui::Widget::TextureResType resType)
{
    auto ret = new (std::nothrow) Item9mm(layer, id);
    if ( ret && ret->init(fileName, resType) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool Item9mm::init(const std::string& fileName, cocos2d::ui::Widget::TextureResType resType)
{
    _resType = resType;
    _fileName = fileName;
    
    if ( resType == cocos2d::ui::Widget::TextureResType::LOCAL )
    {
        _body = Sprite::create(fileName);
    }
    else if ( resType == cocos2d::ui::Widget::TextureResType::PLIST )
    {
        _body = Sprite::createWithSpriteFrameName(fileName);
    }
    
    addChild(_body);
    
    return true;
}


Item9mm* Item9mm::clone() const
{
    return Item9mm::create(_gmxLayer, _id, _fileName, _resType);
}









