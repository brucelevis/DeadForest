//
//  AnimatedFiniteEntity.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 3. 5..
//
//

#include "AnimatedFiniteEntity.hpp"
#include "Game.hpp"
#include "Telegram.hpp"
#include "MessageDispatcher.hpp"
#include "MessageTypes.hpp"
using namespace realtrick::client;
using namespace cocos2d;


AnimatedFiniteEntity::AnimatedFiniteEntity(Game* game) : EntityBase(game),
_base(nullptr),
_interval(0.05f),
_deathTime(0.0f),
_currIdx(0)
{
    setEntityType(EntityType::ENTITY_FINITE);
    _playerType = PlayerType::NEUTRAL;
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


AnimatedFiniteEntity* AnimatedFiniteEntity::create(Game* game, const std::vector<std::string>& frames,
                                                   float deathTime, cocos2d::ui::Widget::TextureResType type)
{
    AnimatedFiniteEntity* ret = new (std::nothrow) AnimatedFiniteEntity(game);
    if ( ret && ret->init(frames, deathTime, type) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
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
    
    _game->sendMessage(0.0, this, this, MessageType::SHOW_NEXT_FRAME, nullptr);
    
    return true;
}


bool AnimatedFiniteEntity::handleMessage(const Telegram& msg)
{
    if ( msg.msg == MessageType::SHOW_NEXT_FRAME )
    {
        if ( _texType == ui::Widget::TextureResType::LOCAL ) _base->setTexture(_frames[_currIdx]);
        else _base->setSpriteFrame(_frames[_currIdx]);
        
        _currIdx ++;
        
        if ( _frames.size() > _currIdx ) _game->sendMessage(_interval, this, this, MessageType::SHOW_NEXT_FRAME, nullptr);
        else _game->sendMessage(_deathTime, this, this, MessageType::REMOVE_SELF, nullptr);
        
        return true;
    }
    else if ( msg.msg == MessageType::REMOVE_SELF )
    {
        _game->removeEntity(this);
        
        return true;
    }
    
    return false;
}









