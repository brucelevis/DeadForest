//
//  ItemBase.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 1. 13..
//
//

#include "ItemBase.hpp"
#include "Game.hpp"
using namespace cocos2d;
using namespace realtrick::client;


ItemBase::ItemBase(Game* game) :
EntityBase(game),
_owner(nullptr),
_amount(0),
_inSlotFrameName("#NO NAME"),
_inSlotSpriteSize(Size::ZERO),
_inGameImage_n(nullptr),
_inGameImage_s(nullptr),
_normalMap(nullptr),
_inSlotImage(nullptr),
_texType(ui::Widget::TextureResType::LOCAL)
{
    ADD_FAMILY_MASK(_familyMask, ITEM_BASE);
    _playerType = PlayerType::NEUTRAL;
    setBoundingRadius(20.0f);
}


ItemBase::~ItemBase()
{
    _owner = nullptr;
    _inGameImage_n = nullptr;
    _inGameImage_s = nullptr;
    _normalMap = nullptr;
    _inSlotImage = nullptr;
}


ItemBase::ItemBase(const ItemBase& rhs) : EntityBase(rhs)
{
    _owner = rhs._owner;
    _amount = rhs._amount;
    _inSlotFrameName = rhs._inSlotFrameName;
    _inSlotSpriteSize = rhs._inSlotSpriteSize;
    _inGameImage_n = nullptr;
    _inGameImage_s = nullptr;
    _normalMap = nullptr;
    _inSlotImage = nullptr;
    _texType = rhs._texType;
}


bool ItemBase::init(const std::string& inGameImage_n, const std::string& inGameImage_s, const std::string& inSlotImage,
                    cocos2d::ui::Widget::TextureResType texResType)
{
    auto normalName = _spriteName;
    for(int i = 0 ; i < 4 ; ++ i) normalName.pop_back(); // remove ".png"
    normalName += "_n.png";
    
    if( texResType == ui::Widget::TextureResType::LOCAL )
    {
        _inGameImage_n = Sprite::create(inGameImage_n);
        _inGameImage_s = Sprite::create(inGameImage_s);
        _inSlotImage = Sprite::create(inSlotImage);
        _normalMap = Sprite::create(normalName);
    }
    else if ( texResType == ui::Widget::TextureResType::PLIST )
    {
        _inGameImage_n = Sprite::createWithSpriteFrameName(inGameImage_n);
        _inGameImage_s = Sprite::createWithSpriteFrameName(inGameImage_s);
        _inSlotImage = Sprite::createWithSpriteFrameName(inSlotImage);
        _normalMap = Sprite::createWithSpriteFrameName(normalName);
    }
    
    _normalMap->setVisible(false);
    _inGameImage_s->setVisible(false);
    _inSlotImage->setVisible(false);
    
    addChild(_inGameImage_n);
    addChild(_inGameImage_s);
    addChild(_normalMap);
    
    setInSlotFrameName(inSlotImage);
    setInSlotSpriteSize(_inSlotImage->getContentSize());
    
    _texType = texResType;
    
    // create box2d's physics body
    b2BodyDef bd;
    bd.type = b2BodyType::b2_staticBody;
    bd.allowSleep = false;
    
    _body = _game->getPhysicsWorld()->CreateBody(&bd);
    
    b2CircleShape shape;
    shape.m_radius = 15;
    
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 10.0f;
    fd.friction = 0.5f;
    fd.restitution = 0.0f;
    fd.isSensor = true;
    _body->CreateFixture(&fd);
    
    return true;
}


void ItemBase::enableNormal(bool enable)
{
    _inGameImage_n->setVisible(!enable);
    _inGameImage_s->setVisible(!enable);
    _normalMap->setVisible(enable);
}









