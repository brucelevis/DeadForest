//
//  HumanGlobalState.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 2. 29..
//
//

#include "HumanGlobalState.hpp"
#include "MessageDispatcher.hpp"
#include "MessageTypes.hpp"
#include "Telegram.hpp"
#include "EntityHuman.hpp"
#include "ui/CocosGUI.h"
#include "Game.hpp"
#include "UiLayer.hpp"
#include "Camera2D.hpp"
#include "AimingNode.hpp"
#include "AnimatedFiniteEntity.hpp"
using namespace cocos2d;
using namespace realtrick::client;

void HumanGlobalState::enter(EntityHuman* human)
{
    
}


void HumanGlobalState::execute(EntityHuman* human)
{
    
}


void HumanGlobalState::exit(EntityHuman* human)
{
    
}


bool HumanGlobalState::onMessage(EntityHuman* human, const Telegram& msg)
{
    if ( msg.msg == MessageType::WEAPON_READY )
    {
        WeaponBase* weapon = static_cast<WeaponBase*>(msg.extraInfo);
        weapon->enableReadyToAttack(true);
        
        return true;
    }
    else if ( msg.msg  == MessageType::HITTED_BY_GUN )
    {
        ReceiverSenderDamage s = *static_cast<ReceiverSenderDamage*>(msg.extraInfo);
        int dam = s.damage;
        human->setBlood( human->getBlood() - dam );
        if ( human->getBlood() <= 0 )
        {
            human->getFSM()->changeState(&HumanBackDeadState::getInstance());
        }
        
        
        AnimatedFiniteEntity* blood = AnimatedFiniteEntity::create(human->getGameManager(), {"blood" + _to_string(random(1,4)) + ".png"},
                                                          random(5, 10), cocos2d::ui::Widget::TextureResType::PLIST);
        blood->setWorldPosition(Vec2(human->getWorldPosition().x + random(-30, 30), human->getWorldPosition().y + random(-30, 30)));
        blood->setScale(0.2f);
        human->getGameManager()->addEntity(blood, Z_ORDER_ITEMS, human->getGameManager()->getNextValidID());
        
        return true;
    }
    else if (msg.msg == MessageType::HITTED_BY_AXE )
    {
        ReceiverSenderDamage s = *static_cast<ReceiverSenderDamage*>(msg.extraInfo);
        int dam = s.damage;
        human->setBlood( human->getBlood() - dam );
        
        if ( human->getBlood() <= 0 )
        {
            human->getFSM()->changeState(&HumanBackDeadState::getInstance());
        }
        
        for(int i = 0 ; i < 5 ; ++ i)
        {
            AnimatedFiniteEntity* blood = AnimatedFiniteEntity::create(human->getGameManager(), {"blood1.png"},
                                                                       random(5, 10), cocos2d::ui::Widget::TextureResType::PLIST);
            blood->setWorldPosition(Vec2(human->getWorldPosition().x + random(-20, 20), human->getWorldPosition().y + random(-20, 20)));
            blood->setScale(0.3f);
            human->getGameManager()->addEntity(blood, Z_ORDER_ITEMS, human->getGameManager()->getNextValidID());
        }
        
        return true;
    }
    else if ( msg.msg == MessageType::PLAY_SOUND )
    {
        SoundSource s =  *static_cast<SoundSource*>(msg.extraInfo);
        float t = (1.0f - (s.position - human->getGameManager()->getGameCamera()->getCameraPos()).getLength() / s.soundRange) * s.volume;
        experimental::AudioEngine::setVolume( experimental::AudioEngine::play2d(s.fileName), t);
        
        return true;
    }
    
    return false;
}









