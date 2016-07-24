//
//  NetworkStream.cpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 4. 3..
//
//

#include "NetworkStream.hpp"
#include "Server.hpp"
#include "Generatedpackets.hpp"
#include "GameManager.hpp"
#include "EntityHuman.hpp"
#include "GameWorld.hpp"
#include "UiLayer.hpp"
#include "Inventory.hpp"
#include "ItemSlot.hpp"
#include "WeaponStatus.hpp"
#include "HumanOwnedAnimations.hpp"
#include "Items.hpp"

using namespace cocos2d;

namespace realtrick
{
    
    void NetworkStream::processPacket(float dt)
    {
        if ( !Server::getInstance().isQueueEmpty() )
        {
            Packet* packet;
            Server::getInstance().dequeue(packet);
            packet->decode();
            
            switch ( packet->type() )
            {
                case PacketType::FISRT_PLAYER_INFOS:
                {
                    auto obj = fpacket::GetPacketFirstPlayerInfos(packet->body());
                    
                    auto own = obj->own();
                    EntityHuman* player = EntityHuman::create(_gameMgr);
                    player->setWorldPosition(Vec2(own->pos_x(), own->pos_y()));
                    player->setUserNickName(own->name()->str());
                    _gameMgr->addDynamicEntity(player, Z_ORDER_HUMAN + 1, own->id());
                    _gameMgr->setPlayer(player);

                    log("[own] id: %d, pos: (%.0f, %.0f), name: %s", own->id(), own->pos_x(), own->pos_y(), own->name()->c_str());
                    
                    
                    auto others = obj->others();
                    uint32_t len = others->Length();
                    log("lenth: %d", static_cast<int>(len));
                    for(auto i = 0 ; i < others->Length() ; ++ i)
                    {
                        auto human = others->Get(i);
                        Vec2 pos(human->pos_x(), human->pos_y());
                        int id = human->id();
                        std::string nickName = human->name()->str();
                        
                        log("[other] id: %d, pos: (%.0f, %.0f), name: %s", id, pos.x, pos.y, nickName.c_str());
                        
                        EntityHuman* player = EntityHuman::create(_gameMgr);
                        player->setWorldPosition(pos);
                        player->setUserNickName(nickName);
                        _gameMgr->addDynamicEntity(player, Z_ORDER_HUMAN + 1, id);
                    }
                    
                    Packet* packet = new Packet();
                    packet->encode(nullptr, 0, PacketType::SUCCESS_LOAD_GAME);
                    Server::getInstance().write(packet);
                    
                    break;
                }
                case PacketType::GAME_START:
                {
                    
                    _gameMgr->pushLogic(0.0, MessageType::LOAD_GAME_COMPLETE, nullptr);
                    
                    break;
                }
                case PacketType::MOVE_JOYSTICK:
                {
                    auto obj = fpacket::GetPacketMoveJoystick(packet->body());
                    
                    int id = obj->id();
                    bool isTouched = obj->is_touched();
                    Vec2 dir(obj->dir_x(), obj->dir_y());
                    
                    log("id: %d, isTouched: %d, dirx: %.f, diry: %.f", id, (int)isTouched, dir.x, dir.y);
                    
                    EntityHuman* player = static_cast<EntityHuman*>(_gameMgr->getEntityFromID(id));
                    player->setMoving(dir);
                    if ( isTouched )
                    {
                        player->addInputMask(HumanBehaviorType::RUN);
                    }
                    else
                    {
                        player->removeInputMask(HumanBehaviorType::RUN);
                    }
                    
                    break;
                }
                    
                default:
                {
                    log("<NetworkStream::processPacket> Invalid Packet Arrived. (%d)", static_cast<int>(packet->type()));
                    break;
                }
            }
            
            CC_SAFE_FREE(packet);
            
        }
    }
    
    
    bool NetworkStream::handleMessage(const Telegram& msg)
    {
        if ( msg.msg == MessageType::LOAD_GAME_PLAYER)
        {
            // player를 제외하고  받는다.
            //_gameMgr->loadMultiGameMap("simple_server_map.txt");
            
            return true;
        }
        else if ( msg.msg == MessageType::LOAD_GAME_COMPLETE )
        {
            
            _gameMgr->getGameWorld()->loadUiLayer();
            _gameMgr->getGameWorld()->resumeGame();
            
            return true;
        }
        else if ( msg.msg == MessageType::MOVE_JOYSTICK_INPUT )
        {
            MoveJoystickData data = *static_cast<MoveJoystickData*>(msg.extraInfo);
            EntityHuman* player = _gameMgr->getPlayerPtr();
            player->setMoving(data.dir);
            
            switch ( data.type )
            {
                case JoystickEx::ClickEventType::BEGAN:
                {
                    player->addInputMask(HumanBehaviorType::RUN);
                    break;
                }
                case JoystickEx::ClickEventType::ENDED:
                {
                    player->removeInputMask(HumanBehaviorType::RUN);
                    break;
                }
                    
                default: break;
            }
            
            bool isTouched = (data.type == JoystickEx::ClickEventType::BEGAN);
            
            flatbuffers::FlatBufferBuilder builder;
            auto obj = fpacket::CreatePacketMoveJoystick(builder, _gameMgr->getPlayerPtr()->getTag(), isTouched, data.dir.x, data.dir.y);
            builder.Finish(obj);
            
            Packet* packet = new Packet();
            packet->encode(builder.GetBufferPointer(), builder.GetSize(), PacketType::MOVE_JOYSTICK);
            Server::getInstance().write(packet);
            
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
        else if ( msg.msg == MessageType::BEZEL_CLICK_INPUT )
        {
            return true;
        }
        else if ( msg.msg == MessageType::PUSH_ITEM_TO_INVENTORY )
        {
            return true;
        }
        else if ( msg.msg == MessageType::PRESS_EQUIP_WEAPON_BUTTON)
        {
            return true;
        }
        else if ( msg.msg == MessageType::PRESS_RELEASE_WEAPON_BUTTON)
        {
            return true;
        }
        else if ( msg.msg == MessageType::PRESS_RELOAD_BUTTON )
        {
            return true;
        }
        else if ( msg.msg == MessageType::RELOAD_COMPLETE )
        {
           return true;
        }
        
        return false;
    }
    
}









