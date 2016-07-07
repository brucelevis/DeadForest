//
//  ItemAxe.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "ItemAxe.hpp"
using namespace cocos2d;
using namespace realtrick;

ItemAxe::ItemAxe(GMXLayer2& layer, int id) :
EntityBase(layer, id)
{
    _type = EntityType::ITEM_AXE;
}


ItemAxe::~ItemAxe()
{
}


ItemAxe* ItemAxe::create(GMXLayer2& layer, int id, const std::string& fileName, cocos2d::ui::Widget::TextureResType resType)
{
    auto ret = new (std::nothrow) ItemAxe(layer, id);
    if ( ret && ret->init(fileName, resType) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool ItemAxe::init(const std::string& fileName, cocos2d::ui::Widget::TextureResType resType)
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










