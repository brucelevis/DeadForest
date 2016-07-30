//
//  EntityRainDrop.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 3. 5..
//
//

#include "EntityRainDrop.hpp"
#include "GameManager.hpp"
#include "Telegram.hpp"
#include "MessageDispatcher.hpp"
#include "MessageTypes.hpp"
#include "Camera2D.hpp"
#include "SizeProtocol.h"
using namespace realtrick::client;
using namespace cocos2d;

EntityRainDrop::EntityRainDrop(GameManager* mgr) : AnimatedFiniteEntity(mgr)
{
}


EntityRainDrop::EntityRainDrop(const EntityRainDrop& rhs) : AnimatedFiniteEntity(rhs)
{
    setEntityType(EntityType::ENTITY_RAIN_DROP);
}


EntityRainDrop::~EntityRainDrop()
{
}


EntityRainDrop* EntityRainDrop::EntityRainDrop::create(GameManager* mgr, const std::vector<std::string>& frames, float deathTime, cocos2d::ui::Widget::TextureResType type)
{
    EntityRainDrop* ret = new (std::nothrow)EntityRainDrop(mgr);
    if ( ret && ret->init(frames, deathTime, type) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool EntityRainDrop::init(const std::vector<std::string> frames, float deathTime, cocos2d::ui::Widget::TextureResType type)
{
    if ( !Node::init() || frames.empty() )
        return false;
    
    _winSize = Size(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT);
    _frames = frames;
    _deathTime = deathTime;
    _texType = type;
    
    _base = Sprite::create();
    addChild(_base);
    
    Dispatch.pushMessage(random(0.0, 3.0), this, this, MessageType::SHOW_NEXT_FRAME, nullptr);
    
    return true;
}


AnimatedFiniteEntity* EntityRainDrop::clone() const
{
    return new EntityRainDrop(*this);
}

bool EntityRainDrop::handleMessage(const Telegram& msg)
{
    if ( msg.msg == MessageType::SHOW_NEXT_FRAME )
    {
        _base->setVisible(true);
        if ( _texType == ui::Widget::TextureResType::LOCAL ) _base->setTexture(_frames[_currIdx]);
        else _base->setSpriteFrame(_frames[_currIdx]);
        
        _currIdx ++;
        
        if ( _frames.size() > _currIdx )
        {
            Dispatch.pushMessage(0.1, this, this, MessageType::SHOW_NEXT_FRAME, nullptr);
        }
        else
        {
            Dispatch.pushMessage(0.0, this, this, MessageType::REPEAT_FRAMES, nullptr);
        }
        
        return true;
    }
    else if ( msg.msg == MessageType::REPEAT_FRAMES )
    {
        _base->setVisible(false);
        Vec2 cameraPos = _gameMgr->getGameCamera()->getCameraPos();
        setWorldPosition(cameraPos + Vec2(random(-_winSize.width * 0.5f, _winSize.width * 0.5f),
                                          random(-_winSize.height * 0.5f, _winSize.height * 0.5f)));
        
        _currIdx = 0;
        
        Dispatch.pushMessage(random(0.0, 3.0), this, this, MessageType::SHOW_NEXT_FRAME, nullptr);
        
        return true;
    }
    
    return false;
}









