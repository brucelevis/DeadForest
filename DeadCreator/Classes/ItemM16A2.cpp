//
//  ItemM16A2.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "ItemM16A2.hpp"
using namespace cocos2d;
using namespace realtrick;

ItemM16A2::ItemM16A2(GMXLayer2& layer, int id) :
EntityBase(layer, id)
{
    _type = EntityType::ITEM_M16A2;
}


ItemM16A2::~ItemM16A2()
{
}


ItemM16A2* ItemM16A2::create(GMXLayer2& layer, int id, const std::string& fileName, cocos2d::ui::Widget::TextureResType resType)
{
    auto ret = new (std::nothrow) ItemM16A2(layer, id);
    if ( ret && ret->init(fileName, resType) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool ItemM16A2::init(const std::string& fileName, cocos2d::ui::Widget::TextureResType resType)
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















