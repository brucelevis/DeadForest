//
//  EditorItemShell.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "EditorItemShell.hpp"
using namespace cocos2d;
using namespace realtrick;

EditorItemShell::EditorItemShell(GMXLayer2& layer, int id) :
EditorEntityBase(layer, id)
{
    _type = EditorEntityType::ITEM_SHELL;
}


EditorItemShell::~EditorItemShell()
{
}


EditorItemShell* EditorItemShell::create(GMXLayer2& layer, int id, const std::string& fileName, cocos2d::ui::Widget::TextureResType resType)
{
    auto ret = new (std::nothrow) EditorItemShell(layer, id);
    if ( ret && ret->init(fileName, resType) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool EditorItemShell::init(const std::string& fileName, cocos2d::ui::Widget::TextureResType resType)
{
    if ( !EditorEntityBase::init() )
        return false;

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









