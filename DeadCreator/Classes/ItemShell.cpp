//
//  ItemShell.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "ItemShell.hpp"
using namespace cocos2d;

ItemShell::ItemShell(GMXLayer2& layer, int id) :
EntityBase(layer, id)
{
    _type = EntityType::ITEM_SHELL;
}


ItemShell::~ItemShell()
{
}


ItemShell* ItemShell::create(GMXLayer2& layer, int id, const std::string& fileName, cocos2d::ui::Widget::TextureResType resType)
{
    auto ret = new (std::nothrow) ItemShell(layer, id);
    if ( ret && ret->init(fileName, resType) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool ItemShell::init(const std::string& fileName, cocos2d::ui::Widget::TextureResType resType)
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


ItemShell* ItemShell::clone() const
{
    return ItemShell::create(_gmxLayer, _id, _fileName, _resType);
}