//
//  BulletShell.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 2. 3..
//
//

#include "BulletShell.hpp"
#include "EntityHuman.hpp"
#include "GameManager.hpp"
using namespace realtrick::client;

BulletShell::BulletShell(GameManager* mgr) : BulletBase(mgr)
{
    setEntityType(BULLET_SHELL);
    setAmount(30);
}


BulletShell::~BulletShell()
{}


BulletShell* BulletShell::create(GameManager* mgr)
{
    BulletShell* ret = new (std::nothrow)BulletShell(mgr);
    if( ret && ret->init("Shell.png", "Shell.png", "Shell.png", cocos2d::ui::Widget::TextureResType::PLIST))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


BulletShell* BulletShell::clone() const
{
    return new BulletShell(*this);
}


void BulletShell::discard()
{
    BulletShell* item = BulletShell::create(_gameMgr);
    item->setAmount( getAmount() );
    item->setPosition(cocos2d::Vec2(_owner->getPosition().x + 50.0f, _owner->getPosition().y));
    _gameMgr->addEntity(item, Z_ORDER_ITEMS, _gameMgr->getNextValidID());
}









