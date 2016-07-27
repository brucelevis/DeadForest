//
//  AnimatedFiniteEntity.cpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 3. 5..
//
//

#include "AnimatedFiniteEntity.hpp"
#include "GameManager.hpp"
#include "Telegram.hpp"
#include "MessageDispatcher.hpp"
#include "MessageTypes.hpp"
using namespace realtrick::client;
using namespace cocos2d;


AnimatedFiniteEntity::AnimatedFiniteEntity(GameManager* mgr) : EntityBase(mgr),
_base(nullptr),
_interval(0.03f),
_deathTime(0.0f),
_currIdx(0)
{
    ADD_FAMILY_MASK(_familyMask, FamilyMask::FINITE_BASE);
}


AnimatedFiniteEntity::~AnimatedFiniteEntity()
{
    
}


AnimatedFiniteEntity::AnimatedFiniteEntity(const AnimatedFiniteEntity& rhs) : EntityBase(rhs)
{
    _base = nullptr;
    _frames = rhs._frames;
    _deathTime = rhs._deathTime;
}


bool AnimatedFiniteEntity::init(const std::vector<std::string> frames, float deathTime, cocos2d::ui::Widget::TextureResType type)
{
    if ( !Node::init() || frames.empty() )
        return false;
    
    _frames = frames;
    _deathTime = deathTime;
    _texType = type;
    
    _base = Sprite::create();
    addChild(_base);
    
    Dispatch.pushMessage(0.0, this, this, MessageType::SHOW_NEXT_FRAME, nullptr);
    
    return true;
}


bool AnimatedFiniteEntity::handleMessage(const Telegram& msg)
{
    if ( msg.msg == MessageType::SHOW_NEXT_FRAME )
    {
        if ( _texType == ui::Widget::TextureResType::LOCAL ) _base->setTexture(_frames[_currIdx]);
        else _base->setSpriteFrame(_frames[_currIdx]);
        
        _currIdx ++;
        
        if ( _frames.size() > _currIdx )
        {
            Dispatch.pushMessage(0.1, this, this, MessageType::SHOW_NEXT_FRAME, nullptr);
        }
        else
        {
            Dispatch.pushMessage(_deathTime, this, this, MessageType::REMOVE_SELF, nullptr);
        }
        
        return true;
    }
    else if ( msg.msg == MessageType::REMOVE_SELF )
    {
        _gameMgr->removeEntity(getTag());
        
        return true;
    }
    
    return false;
}









