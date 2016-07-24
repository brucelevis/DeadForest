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

bool Room::setPlayer(Session* session)
{
    cout << endl;
    cout << "==========setPlayer==========" << endl;
    cout << "=>Room:" << _roomId << " setPlayer:" << session->getPid() << endl;
    if (session == nullptr)
    {
        return true;
    }
    
    if (_pSessionGroup.size() < 32)
    {
        _pSessionGroup.insert(map<int, realtrick::Session*>::value_type(session->getPid(), session));
        refreshTimer();
        return true;
    }
    
    return false;
}

Session* Room::getPlayer(int pid)
{
    std::map<int, realtrick::Session*>::iterator findSession = _pSessionGroup.find(pid);
    
    if (_pSessionGroup.end() != findSession) {
        return findSession->second;
    }
    return NULL;
}


void Room::deletePlayer(int pid)
{
    std::map<int, realtrick::Session*>::iterator findSession = _pSessionGroup.find(pid);
    
    if (_pSessionGroup.end() != findSession) {
        Session* delSession = findSession->second;
        deleteIdConvertGroup(delSession->getPid());
        _pSessionGroup.erase(findSession);
        refreshTimer();
    }
};

void Room::setLoadGamePlayer(int pid)
{
    _gameManager->setLoadGamePlayer(getPidToVid(pid));
}

bool Room::isEmptyRoom()
{
    return _pSessionGroup.empty();
}

void Room::sendPlayerSuccessSearchGame()
{
    cout << endl;
    cout << "==========sendPlayerSuccessSearchGame==========" << endl;
    if (getRoomState() == static_cast<int> (RoomState::INGAME))
    {
        return;
    }
    
    cout << "=>_pSessionGroup size:" << _pSessionGroup.size() << endl;
    if (0<_pSessionGroup.size() && getRoomState() == static_cast<int> (RoomState::WAIT))
    {
        cout << "=>roomId:" << _roomId << endl;
        setRoomState(static_cast<int> (RoomState::READY));
        
        for (std::map<int, realtrick::Session*>::const_iterator tmpPlayer = _pSessionGroup.begin(); tmpPlayer != _pSessionGroup.end(); ++tmpPlayer)
        {
            if (tmpPlayer->second->getRoomid() == _roomId)
            {
                _server->sendSuccessSearchGame(tmpPlayer->second);
            }
        }
        
        initGame();
        
        return;
    }else{
        refreshTimer();
    }
}

void Room::refreshTimer()
{
    cout << endl;
    cout << "==========refreshTimer==========" << endl;
    int targetTime = _server->getTickCount() + 5000;
    _server->enqueueTimer(_roomId, targetTime, OP_ROOM_TIMER);
    _room_timer = targetTime;
}

int Room::getNextValidID()
{
    cout << "[getNextValidID]" << endl;
    return _validID++;
}

void Room::initGame()
{
    cout << endl;
    cout << "==========initGame==========" << endl;
    _gameManager = new GameManager(this);
    _gameManager->loadGameMap("simple_server_map.txt");
    
}

void Room::updateGame()
{
    cout << endl;
    cout << "==========updateGame==========" << endl;
    
    float dt = 0.01;
    _gameManager->update(dt);
    
    _gameManager->adjustPosition();
    
    int targetTime = getServer()->getTickCount() + 100;
    getServer()->enqueueTimer(getRoomId(), targetTime, OP_GAME_TIMER);
}

void Room::moveJoystick(FlatbufferPacket* packet, int pid)
{
    cout << "[moveJoystick]" << endl;
    _gameManager->moveJoystick(packet, pid);
}
