//
//  Camera2D.cpp
//  TheDeadForest
//
//  Created by mac on 2016. 2. 14..
//
//

#include "Camera2D.hpp"
using namespace realtrick::client;
using namespace cocos2d;

Camera2D::Camera2D(GameManager* mgr) :
_worldSize(Size::ZERO),
_gameMgr(mgr)
{}


Camera2D::~Camera2D()
{}


Camera2D* Camera2D::create(GameManager* mgr)
{
    Camera2D* ret = new (std::nothrow)Camera2D(mgr);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool Camera2D::init()
{
    if ( !Node::init() )
        return false;
    
    _worldSize = Director::getInstance()->getVisibleSize();
    
    return true;
}









