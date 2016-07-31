//
//  Bullet556mm.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 2. 3..
//
//

#include "Bullet556mm.hpp"
#include "EntityHuman.hpp"
#include "Game.hpp"
using namespace realtrick::client;

Bullet556mm::Bullet556mm(Game* game) : BulletBase(game)
{
    setEntityType(BULLET_556MM);
    setAmount(90);
}


Bullet556mm::~Bullet556mm()
{}


Bullet556mm* Bullet556mm::create(Game* game)
{
    Bullet556mm* ret = new (std::nothrow)Bullet556mm(game);
    if( ret && ret->init("5_56mm.png","5_56mm.png","5_56mm.png",cocos2d::ui::Widget::TextureResType::PLIST))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


Bullet556mm* Bullet556mm::clone() const
{
    return new Bullet556mm(*this);
}


void Bullet556mm::discard()
{
    Bullet556mm* item = Bullet556mm::create(_game);
    item->setAmount( getAmount() );
    item->setPosition(cocos2d::Vec2(_owner->getPosition().x + 50.0f, _owner->getPosition().y));
    _game->addEntity(item, Z_ORDER_ITEMS, _game->getNextValidID());
}









