//
//  AimingNode.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 5. 16..
//
//

#include "AimingNode.hpp"

using namespace realtrick::client;
using namespace cocos2d;


AimingNode::AimingNode(GameManager* mgr):
_gameMgr(mgr),
_crossHair(nullptr),
_outter(nullptr),
_inner(nullptr),
_isOutterOn(false),
_isInnerOn(false)
{
}


AimingNode* AimingNode::create(GameManager* mgr)
{
    auto ret = new (std::nothrow) AimingNode(mgr);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool AimingNode::init()
{
    if ( !Node::init() )
        return false;
    
    _crossHair = Sprite::create("hit_point.png");
    _crossHair->setVisible(false);
    addChild(_crossHair);
    
    _outter = Sprite::create("outter.png");
    _outter->setVisible(true);
    _outter->setScale(0.5f);
    _outter->setOpacity(0);
    addChild(_outter);
    
    _inner = Sprite::create("inner.png");
    _inner->setVisible(true);
    _inner->setScale(0.5f);
    _inner->setOpacity(0);
    addChild(_inner);
    
    return true;
}


void AimingNode::runHitAction()
{
    _crossHair->stopAllActions();
    _crossHair->setScale(0.4f);
    _crossHair->runAction(Sequence::create(Show::create(),
                                           Place::create(Vec2(_range + random(-10.0f, 10.0f),random(-10.0f, 10.0f))),
                                           ScaleTo::create(0.07f, 0.7f),
                                           ScaleTo::create(0.05f, 0.2f),
                                           Hide::create(),
                                           nullptr));
}


void AimingNode::setRange(float range)
{
    _range = range;
    
    _crossHair->setPosition(Vec2(range, 0.0f));
    _outter->setPosition(Vec2(range, 0.0f));
    _inner->setPosition(Vec2(range, 0.0f));
}


void AimingNode::showOutter()
{
    if ( _isOutterOn ) return ;
    _outter->runAction(FadeTo::create(0.15, 255));
    _isOutterOn = true;
}


void AimingNode::hideOutter()
{
    if ( !_isOutterOn ) return ;
    _outter->runAction(FadeTo::create(0.15, 0));
    _isOutterOn = false;
}


void AimingNode::showInner()
{
    if ( _isInnerOn ) return ;
    _inner->runAction(FadeTo::create(0.15, 255));
    _isInnerOn = true;
}


void AimingNode::hideInner()
{
    if ( !_isInnerOn ) return ;
    _inner->runAction(FadeTo::create(0.15, 0));
    _isInnerOn = false;

}









