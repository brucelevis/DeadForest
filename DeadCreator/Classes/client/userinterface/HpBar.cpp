//
//  HpBar.cpp
//  TheDeadForest
//
//  Created by mac on 2016. 5. 16..
//
//

#include "HpBar.hpp"
#include "GameManager.hpp"

using namespace realtrick::client;
using namespace cocos2d;

HpBar::HpBar(GameManager* mgr) :
_gameMgr(mgr)
{
}


HpBar* HpBar::create(GameManager* mgr)
{
    auto ret = new (std::nothrow) HpBar(mgr);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_FREE(ret);
    return nullptr;
}


bool HpBar::init()
{
    if ( !Node::init() )
        return false;
    
    _bar = Sprite::create("hpBar.png");
    _bar->setAnchorPoint(Vec2(0.0f, 0.5f));
    addChild(_bar);
    
    _bundle = Sprite::create("hpBundle.png");
    _bundle->setAnchorPoint(Vec2(0.0f, 0.5f));
    addChild(_bundle);
    
    return true;
}


void HpBar::setHitPoint(float h)
{

}









