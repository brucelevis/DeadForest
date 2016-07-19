//
//  EditorItem556mm.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "EditorItem556mm.hpp"
using namespace cocos2d;
using namespace realtrick;

EditorItem556mm::EditorItem556mm(GMXLayer& layer, int id) :
EditorEntityBase(layer, id)
{
    _type = EntityType::BULLET_556MM;
    setName("5.56mm");
}


EditorItem556mm::~EditorItem556mm()
{
}


EditorItem556mm* EditorItem556mm::create(GMXLayer& layer, int id, const std::string& fileName,
                                         cocos2d::ui::Widget::TextureResType resType)
{
    auto ret = new (std::nothrow) EditorItem556mm(layer, id);
    if ( ret && ret->init(fileName, resType) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool EditorItem556mm::init(const std::string& fileName, cocos2d::ui::Widget::TextureResType resType)
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









