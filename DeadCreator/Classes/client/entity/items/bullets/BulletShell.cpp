//
//  BulletShell.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 2. 3..
//
//

#include "BulletShell.hpp"
#include "HumanBase.hpp"
#include "Game.hpp"
using namespace realtrick::client;

BulletShell::BulletShell(Game* game) : BulletBase(game)
{
    setEntityType(BULLET_SHELL);
    setAmount(30);
    _spriteName = "Shell.png";
}


BulletShell::~BulletShell()
{}


BulletShell* BulletShell::create(Game* game)
{
    BulletShell* ret = new (std::nothrow)BulletShell(game);
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
}









