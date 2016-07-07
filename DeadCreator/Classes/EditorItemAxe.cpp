//
//  EditorItemAxe.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "EditorItemAxe.hpp"
using namespace cocos2d;
using namespace realtrick;

EditorItemAxe::EditorItemAxe(GMXLayer2& layer, int id) :
EditorEntityBase(layer, id)
{
    _type = EditorEntityType::ITEM_AXE;
}


EditorItemAxe::~EditorItemAxe()
{
}


EditorItemAxe* EditorItemAxe::create(GMXLayer2& layer, int id, const std::string& fileName, cocos2d::ui::Widget::TextureResType resType)
{
    auto ret = new (std::nothrow) EditorItemAxe(layer, id);
    if ( ret && ret->init(fileName, resType) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool EditorItemAxe::init(const std::string& fileName, cocos2d::ui::Widget::TextureResType resType)
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










