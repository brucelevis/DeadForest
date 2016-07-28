//
//  Bullet9mm.cpp
//  TheDeadForest
//
//  Created by mac on 2016. 2. 3..
//
//

#include "Bullet9mm.hpp"
#include "EntityHuman.hpp"
#include "GameManager.hpp"
using namespace realtrick::client;

Bullet9mm::Bullet9mm(GameManager* mgr) : BulletBase(mgr)
{
    setEntityType(BULLET_9MM);
    setAmount(21);
}


Bullet9mm::~Bullet9mm()
{}


Bullet9mm* Bullet9mm::create(GameManager* mgr)
{
    Bullet9mm* ret = new (std::nothrow)Bullet9mm(mgr);
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
    Bullet9mm* item = Bullet9mm::create(_gameMgr);
    item->setAmount( getAmount() );
    item->setPosition(cocos2d::Vec2(_owner->getPosition().x + 50.0f, _owner->getPosition().y));
    _gameMgr->addEntity(item, Z_ORDER_ITEMS, _gameMgr->getNextValidID());
}









