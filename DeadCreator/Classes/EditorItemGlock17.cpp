//
//  EditorItemGlock17.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "EditorItemGlock17.hpp"
using namespace cocos2d;
using namespace realtrick;

EditorItemGlock17::EditorItemGlock17(GMXLayer2& layer, int id) :
EditorEntityBase(layer, id)
{
    _type = EntityType::ITEM_GLOCK17;
}


EditorItemGlock17::~EditorItemGlock17()
{
}


EditorItemGlock17* EditorItemGlock17::create(GMXLayer2& layer, int id, const std::string& fileName, cocos2d::ui::Widget::TextureResType resType)
{
    auto ret = new (std::nothrow) EditorItemGlock17(layer, id);
    if ( ret && ret->init(fileName, resType) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool EditorItemGlock17::init(const std::string& fileName, cocos2d::ui::Widget::TextureResType resType)
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















