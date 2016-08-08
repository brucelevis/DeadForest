#include "Room.h"

using namespace realtrick;
using namespace std;

int Room::getRoomId()
{
    return _roomId;
}

void Room::setRoomState(int state)
{
    _roomState = state;
}

int Room::getRoomState()
{
    return _roomState;
}

int Room::getRoomMode()
{
    return _roomMode;
}

int Room::getMaxRoomPlayer()
{
    return _maxRoomPlayer;
}


void Room::setLoadGamePlayer(int pid)
{
    _gameManager->setLoadGamePlayer(getPidToVid(pid));
}


void Room::sendPlayerSuccessSearchGame()
{
    setRoomState(static_cast<int> (RoomState::READY));
    
    initGame();
}

void Room::refreshTimer()
{
    /*
    int targetTime = getTickCount() + 5000;
    _server->enqueueTimer(_roomId, targetTime, OP_ROOM_TIMER);
    _room_timer = targetTime;
     */
}

int Room::getNextValidID()
{
    return _validID++;
}

void Room::initGame()
{
    _gameManager = new GameManager(this);
    _gameManager->loadGameMap("simple_server_map.txt");
    
}

void Room::updateGame()
{
    
    float dt = 0.01;
    _gameManager->update(dt);
    /*
    int targetTime = getServer()->getTickCount() + 1000;
    getServer()->enqueueTimer(getRoomId(), targetTime, OP_GAME_TIMER);
     */
}

void Room::moveJoystick(FlatbufferPacket* packet, int pid)
{
    _gameManager->moveJoystick(packet, pid);
}

