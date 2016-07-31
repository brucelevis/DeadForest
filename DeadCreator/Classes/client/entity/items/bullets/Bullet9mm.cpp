//
//  Bullet9mm.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 2. 3..
//
//

#include "Bullet9mm.hpp"
#include "EntityHuman.hpp"
#include "Game.hpp"
using namespace realtrick::client;

Bullet9mm::Bullet9mm(Game* game) : BulletBase(game)
{
    setEntityType(BULLET_9MM);
    setAmount(21);
}


Bullet9mm::~Bullet9mm()
{}


Bullet9mm* Bullet9mm::create(Game* game)
{
    Bullet9mm* ret = new (std::nothrow)Bullet9mm(game);
    if( ret && ret->init("9mm.png", "9mm.png", "9mm.png", cocos2d::ui::Widget::TextureResType::PLIST))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


Bullet9mm* Bullet9mm::clone() const
{
    return new Bullet9mm(*this);
}


void Bullet9mm::discard()
{
    Bullet9mm* item = Bullet9mm::create(_game);
    item->setAmount( getAmount() );
    item->setPosition(cocos2d::Vec2(_owner->getPosition().x + 50.0f, _owner->getPosition().y));
    _game->addEntity(item, Z_ORDER_ITEMS, _game->getNextValidID());
}









