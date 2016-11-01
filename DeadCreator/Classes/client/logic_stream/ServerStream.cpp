//
//  ServerStream.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 8. 11..
//
//

#define _ENABLE_ATOMIC_ALIGNMENT_FIX

#include "ServerStream.hpp"
#include "Game.hpp"
#include "EntityPlayer.hpp"
#include "UiLayer.hpp"
#include "HumanOwnedAnimations.hpp"
#include "Items.hpp"
#include "InputCommands.hpp"
#include "GameServer.hpp"
using namespace cocos2d;
using namespace realtrick::client;


void ServerStream::update(float dt)
{
    if ( !GameServer::getInstance().isQueueEmpty() )
    {
        Packet* packet;
        GameServer::getInstance().dequeue(packet);
        packet->decode();
        
        switch ( packet->type() )
        {
            case PacketType::LOAD_GMXFILE:
            {
                const char* binary = "";
                _game->loadGMXFileFromBinary(binary);
                
                break;
            }
            default:
            {
                log("<ConnectScene::update> Invalid Packet Arrived. (%d)", static_cast<int>(packet->type()));
                break;
            }
        }
        
        CC_SAFE_DELETE(packet);
    }
}


bool ServerStream::handleMessage(const Telegram& msg)
{
    // loading methods
    if ( msg.msg == MessageType::LOAD_GAME_PLAYER)
    {
        return true;
    }
    
    else if ( msg.msg == MessageType::LOAD_GAME_COMPLETE )
    {
        return true;
    }
    
    // input commands
    else if ( msg.msg == MessageType::MOVE_JOYSTICK_INPUT )
    {
        return true;
    }
    
    else if ( msg.msg == MessageType::ATTACK_JOYSTICK_INPUT )
    {
        return true;
    }
    
    else if ( msg.msg == MessageType::BEZEL_DIRECTION_TRIGGERED )
    {
        return true;
    }
    
    else if ( msg.msg == MessageType::PRESS_RELOAD_BUTTON )
    {
        return true;
    }
    
    // callback funcs
    else if ( msg.msg == MessageType::PUSH_ITEM_TO_INVENTORY )
    {
        return true;
    }
    
    else if ( msg.msg == MessageType::PRESS_EQUIP_WEAPON_BUTTON )
    {
        return true;
    }
    
    else if ( msg.msg == MessageType::PRESS_RELEASE_WEAPON_BUTTON )
    {
        return true;
    }
    
    else if ( msg.msg == MessageType::RELOAD_COMPLETE )
    {
        return true;
    }
    
    return false;
}









