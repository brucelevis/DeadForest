//
//  EntityZombie.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 2..
//
//

#include "EntityZombie.hpp"
#include "ZombieStates.hpp"
#include "StringHelper.hpp"
#include "Game.hpp"
#include "AnimatedFiniteEntity.hpp"
#include "ZombieBrain.hpp"
#include "UiLayer.hpp"
using namespace realtrick::client;
using namespace cocos2d;


EntityZombie::EntityZombie(Game* game) : HumanBase(game)
{
    setEntityType(EntityType::ENTITY_ZOMBIE);
    setRunSpeed(150.0f);
    setWalkSpeed(75.0f);
    _maxBlood = 100;
    _blood = _maxBlood;
}


EntityZombie::~EntityZombie()
{
    CC_SAFE_DELETE(_FSM);
    CC_SAFE_DELETE(_brain);
}


bool EntityZombie::init()
{
    if ( !HumanBase::init() )
        return false;
    
    _FSM = new StateMachine(this);
    _FSM->setCurrState(&ZombieIdleLoop::getInstance());
    _FSM->changeState(&ZombieIdleLoop::getInstance());
    
    return true;
}


EntityZombie* EntityZombie::create(Game* game)
{
    auto ret = new (std::nothrow) EntityZombie(game);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


void EntityZombie::update(float dt)
{
    HumanBase::update(dt);
}


bool EntityZombie::handleMessage(const Telegram& msg)
{
    bool ret = false;
    
    ret = HumanBase::handleMessage(msg);
    
    if ( msg.msg == MessageType::HITTED_BY_GUN || msg.msg == MessageType::HITTED_BY_AXE || msg.msg == MessageType::HITTED_BY_FIST )
    {
        ReceiverSenderDamage* d = static_cast<ReceiverSenderDamage*>(msg.extraInfo);
        if ( _blood > 0 ) _blood -= d->damage;
        if ( _blood <= 0 && isAlive() )
        {
            if ( d->sender->getTag() == _game->getPlayerPtr()->getTag() )
            {
                if ( msg.msg == MessageType::HITTED_BY_GUN)
                {
                    SoundSource s;
                    s.fileName = "kill_sound.mp3";
                    s.position = static_cast<HumanBase*>(d->sender)->getWorldPosition();
                    s.soundRange = 100.0f;
                    s.volume = 1.0f;
                    _game->pushLogic(0.0, MessageType::PLAY_SOUND, &s);
                }
                
                std::string text = "kill zombie!";
                _game->sendMessage(0.0, d->sender, nullptr, MessageType::DISPLAY_TEXT, &text);
            }

            this->getFSM()->changeState(&ZombieDead::getInstance());
        }
        
        // only apply to player
        if ( _uiLayer )
        {
            float h = _blood / static_cast<float>(_maxBlood);
            h = cocos2d::clampf(h, 0.0f, 1.0f);
            _uiLayer->setHitPoint(h);
        }
        
        ret = true;
    }
    
    else if ( msg.msg == MessageType::HIT )
    {
        // only apply to player
        if ( _uiLayer ) _uiLayer->runCrossHairEffect("hit");
        
        ret = true;
    }
    
    else if ( msg.msg == MessageType::NO_HIT )
    {
        // only apply to player
        if ( _uiLayer ) _uiLayer->runCrossHairEffect("fire");
        
        ret = true;
    }
    
    return ret;
}


void EntityZombie::suicide()
{
    if ( _FSM ) _FSM->changeState(&ZombieDead::getInstance());
}









