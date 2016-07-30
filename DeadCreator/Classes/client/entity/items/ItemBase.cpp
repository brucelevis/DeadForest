//
//  ItemBase.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 1. 13..
//
//

#include "ItemBase.hpp"
using namespace cocos2d;
using namespace realtrick::client;

ItemBase::ItemBase(GameManager* mgr) :
EntityBase(mgr),
_owner(nullptr),
_amount(0),
_inSlotFrameName("#NO NAME"),
_inSlotSpriteSize(Size::ZERO),
_inGameImageName_n("#NO NAME"),
_inGameImageName_s("#NO NAME"),
_inGameImage_n(nullptr),
_inGameImage_s(nullptr),
_inSlotImage(nullptr),
_inGameNormal(nullptr),
_texType(ui::Widget::TextureResType::LOCAL)
{
    ADD_FAMILY_MASK(_familyMask, ITEM_BASE);
    setBoundingRadius(20.0f);
}


ItemBase::~ItemBase()
{
    _owner = nullptr;
    _inGameImage_n = nullptr;
    _inGameImage_s = nullptr;
    _inSlotImage = nullptr;
    _inGameNormal = nullptr;
}

ItemBase::ItemBase(const ItemBase& rhs) : EntityBase(rhs)
{
    _owner = rhs._owner;
    _amount = rhs._amount;
    _inSlotFrameName = rhs._inSlotFrameName;
    _inSlotSpriteSize = rhs._inSlotSpriteSize;
    _inGameImageName_n = rhs._inGameImageName_n;
    _inGameImageName_s = rhs._inGameImageName_s;
    _inGameImage_n = nullptr;
    _inGameImage_s = nullptr;
    _inSlotImage = nullptr;
    _inGameNormal = nullptr;
    _texType = rhs._texType;
}


bool ItemBase::init(const std::string& inGameImage_n, const std::string& inGameImage_s, const std::string& inSlotImage, cocos2d::ui::Widget::TextureResType texResType)
{
    std::string normalImagePath = inGameImage_n;
    std::string::size_type idx = normalImagePath.find_first_of('.');
    if ( idx != std::string::npos ) normalImagePath.insert(idx, "_n");
    
    if( texResType == ui::Widget::TextureResType::LOCAL )
    {
        _inGameImage_n = Sprite::create(inGameImage_n);
        _inGameImage_s = Sprite::create(inGameImage_s);
        _inSlotImage = Sprite::create(inSlotImage);
        _inGameNormal = Sprite::create(normalImagePath);
    }
    else if ( texResType == ui::Widget::TextureResType::PLIST )
    {
        _inGameImage_n = Sprite::createWithSpriteFrameName(inGameImage_n);
        _inGameImage_s = Sprite::createWithSpriteFrameName(inGameImage_s);
        _inSlotImage = Sprite::createWithSpriteFrameName(inSlotImage);
        _inGameNormal = Sprite::createWithSpriteFrameName(normalImagePath);
    }
    
    _inGameImage_s->setVisible(false);
    _inSlotImage->setVisible(false);
    _inGameNormal->setVisible(false);
    
    addChild(_inGameImage_n);
    addChild(_inGameImage_s);
    addChild(_inGameNormal);
    
    setInSlotFrameName(inSlotImage);
    setInSlotSpriteSize(_inSlotImage->getContentSize());
    setInGameFrameName_n(inGameImage_n);
    setInGameFrameName_s(inGameImage_s);
    
    _texType = texResType;
    
    return true;
}









