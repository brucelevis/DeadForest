//
//  main.cpp
//  TheDeadForestTestClient
//
//  Created by ByoungKwon Do on 2016. 6. 20..
//  Copyright © 2016년 ByoungKwon Do. All rights reserved.
//

#include "Server.hpp"
#include "GeneratedPackets.hpp"

using namespace realtrick;
using namespace std;

int pid = -1;
int rMode = 1;
int rid = 0;

void connectServer(){
    cout<<"<CONNECT_SERVER>"<<endl;
    Server::getInstance().connect();
}

void searchGame(){
    cout<<"<SEARCH_GAME>"<<endl;
    flatbuffers::FlatBufferBuilder builder;
    auto obj = fpacket::CreatePacketDoSearchGame(builder, rMode);
    builder.Finish(obj);
    
    Packet* packet = new Packet();
    packet->encode(builder.GetBufferPointer(), builder.GetSize(), PacketType::DO_SEARCH_GAME);
    Server::getInstance().write(packet);
}

void cancelGame(){
    cout<<"<CANCEL_GAME>"<<endl;
    flatbuffers::FlatBufferBuilder builder;
    auto obj = fpacket::CreatePacketCancelSearchGame(builder, rid);
    builder.Finish(obj);
    
    Packet* packet = new Packet();
    packet->encode(builder.GetBufferPointer(), builder.GetSize(), PacketType::CANCEL_SEARCH_GAME);
    Server::getInstance().write(packet);
}

void successLoadGame(){
    cout<<"<SUCCESS_LOAD_GAME>"<<endl;
    Packet* packet = new Packet();
    packet->encode(nullptr, 0, PacketType::SUCCESS_LOAD_GAME);
    Server::getInstance().write(packet);
}

void moveJoystick(){
    
    cout<<"<MOVE_JOYSTICK>"<<endl;
    /*
    bool isTouched = (data.type == JoystickEx::ClickEventType::BEGAN);
    
    flatbuffers::FlatBufferBuilder builder;
    auto obj = fpacket::CreatePacketMoveJoystick(builder, _gameMgr->getPlayerPtr()->getTag(), isTouched, data.dir.x, data.dir.y);
    builder.Finish(obj);
    
    Packet* packet = new Packet();
    packet->encode(builder.GetBufferPointer(), builder.GetSize(), PacketType::MOVE_JOYSTICK);
    Server::getInstance().write(packet);
     */
}
int main()
{
    
    connectServer();
    
    while(true){
        
        if ( !Server::getInstance().isQueueEmpty() )
        {
            Packet* packet;
            Server::getInstance().dequeue(packet);
            packet->decode();
            
            switch ( packet->type() )
            {
                case PacketType::LOGIN_SUCCESS:
                {
                    auto obj = fpacket::GetPacketLoginSuccess(packet->body());
                    pid = obj->pid();

                    cout<<"<LOGIN_SUCCESS> arrived pid: "<<obj->pid()<<endl;
                    cout<<"<LOGIN_SUCCESS> arrived userName: "<<obj->id()->c_str()<<endl;
                    searchGame();
                    break;
                }
                case PacketType::LOGIN_FAIL:
                {
                    cout<<"<LOGIN_FAIL>"<<endl;
                    break;
                }
                case PacketType::SIGN_UP:
                {
                    cout<<"<SIGN_UP>"<<endl;
                    break;
                }
                case PacketType::SUCCESS_SEARCH_GAME:
                {
                    cout<<"<SUCCESS_SEARCH_GAME>"<<endl;
                    auto obj = fpacket::GetPacketSuccessSearchGame(packet->body());
                    rid = obj->roomId();
                    cout<<"<SUCCESS_SEARCH_GAME> room id: "<<rid<<endl;
                    successLoadGame();
                    break;
                }
                case PacketType::FAIL_SEARCH_GAME:
                {
                    cout<<"<FAIL_SEARCH_GAME>"<<endl;
                    break;
                }
                case PacketType::FISRT_PLAYER_INFOS:
                {
                    cout<<"<FISRT_PLAYER_INFOS>"<<endl;
                    auto obj = fpacket::GetPacketFirstPlayerInfos(packet->body());
                    auto own = obj->own();
                    cout<<"[own] id: "<<own->id()<<", pos: ("<<own->pos_x()<<", "<<own->pos_y()<<"), name: "<<own->name()->c_str()<<endl;
                    successLoadGame();
                    break;
                }
                case PacketType::GAME_START:
                {
                    cout<<"<GAME_START>"<<endl;
                    break;
                }
                    
                default:
                {
                    cout<<"<ConnectScene::update> Invalid Packet Arrived. ("<< static_cast<int>(packet->type())<<")"<<endl;
                    break;
                }
            }
        }
        
    }
    return 0;
}

