//
//  FiniteEntityBase.cpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 3. 2..
//
//

#include "FiniteEntityBase.hpp"
#include "MessageDispatcher.hpp"
#include "MessageTypes.hpp"
#include "Telegram.hpp"
#include "GameManager.hpp"
using namespace realtrick::client;
using namespace cocos2d;

FiniteEntityBase::FiniteEntityBase(GameManager* mgr) : EntityBase(mgr)
{
    ADD_FAMILY_MASK(_familyMask, FINITE_BASE);
}


FiniteEntityBase::FiniteEntityBase(const FiniteEntityBase& rhs) : EntityBase(rhs)
{
    _deathTime = rhs._deathTime;
    _inGameImage = nullptr;
}


FiniteEntityBase::~FiniteEntityBase() {}


bool FiniteEntityBase::init(const std::string& inGameImagePath, float deathTime, cocos2d::ui::Widget::TextureResType type)
{
    if ( !Node::init() )
        return false;
    
    _deathTime = deathTime;
    
    if ( type == ui::Widget::TextureResType::LOCAL)
    {
        _inGameImage = Sprite::create(inGameImagePath);
    }
    else
    {
        _inGameImage = Sprite::createWithSpriteFrameName(inGameImagePath);
    }
    
    addChild(_inGameImage, 1);
    
    Dispatch.pushMessage(deathTime, this, this, MessageType::REMOVE_SELF, nullptr);
    
    return true;
}


bool FiniteEntityBase::handleMessage(const Telegram& msg)
{
    if ( msg.msg == MessageType::REMOVE_SELF )
    {
        _gameMgr->removeEntity(getTag());
        return true;
    }
    
    return false;
}









