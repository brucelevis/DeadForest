//
//  main.cpp
//  TheDeadForestTestClient
//
//  Created by ByoungKwon Do on 2016. 6. 20..
//  Copyright © 2016년 ByoungKwon Do. All rights reserved.
//

#include <queue>

#include "Server.hpp"
#include "GeneratedPackets.hpp"
#include "GameManager.hpp"
#include "Room.h"

using namespace realtrick;
using namespace std;

#define OP_ROOM_TIMER 1
#define OP_GAME_TIMER 2

struct eventToken {
    int eventId;
    unsigned int targetTime;
    int eventType;
};

class mycomp {
    bool reverse;
public:
    mycomp() {}
    bool operator() (const eventToken lhs, const eventToken rhs) const
    {
        return (lhs.targetTime > rhs.targetTime);
    }
};

priority_queue<eventToken, vector<eventToken>, mycomp> _timerQueue;
boost::thread* _timerThread;
GameManager* _gameManager;
Room *newRoom;

int pid = -1;
int rMode = 1;
int rid = 0;
int gid = 0;


#pragma mark - Network Method

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

/**
 *@param True/False 터치 시작/종료
 */
void moveJoystick(bool isTouched, float x, float y){
    
    cout<<"<MOVE_JOYSTICK>"<<endl;
    
    flatbuffers::FlatBufferBuilder builder;
    auto obj = fpacket::CreatePacketMoveJoystick(builder, gid, isTouched, x, y);
    builder.Finish(obj);
    
    Packet* packet = new Packet();
    packet->encode(builder.GetBufferPointer(), builder.GetSize(), PacketType::MOVE_JOYSTICK);
    Server::getInstance().write(packet);
    
}


#pragma mark - Timer Method

unsigned int getTickCount()
{
    struct timeval gettick;
    gettimeofday(&gettick, NULL);
    
    return static_cast<unsigned int>(gettick.tv_sec*1000 + gettick.tv_usec/1000);
}

void enqueueTimer(int id, unsigned int time, int type)
{
    _timerQueue.push(eventToken{ id, time, type });
}

#pragma mark - Thread Methods

void timerThread()
{
    while (true)
    {
        while  (false == _timerQueue.empty())
        {
            unsigned int curTickCount = getTickCount();
            if (_timerQueue.top().targetTime > curTickCount) break;
            eventToken et = _timerQueue.top();
            
            if(et.eventType == OP_ROOM_TIMER){
                cout << "timerThread: OP_ROOM_TIMER" <<endl;
            }else if(et.eventType == OP_GAME_TIMER){
                cout << "timerThread: OP_GAME_TIMER" <<endl;
                
            }
            _timerQueue.pop();
        }
    }
}

int main()
{
    _timerThread = new boost::thread(boost::bind(&timerThread));
    
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
                    
                    if(newRoom == nullptr){
                        newRoom = new Room(rid, rMode);
                        newRoom->setRoomState(static_cast<int> (RoomState::WAIT));
                        newRoom->sendPlayerSuccessSearchGame();
                    }
                    
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
                    gid = own->id();
                    
                    
                    successLoadGame();
                    
                    break;
                }
                case PacketType::GAME_START:
                {
                    cout<<"<GAME_START>"<<endl;
                    
                    moveJoystick(true, 1, 1);
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
    
    _timerThread->join();
    
    return 0;
}

