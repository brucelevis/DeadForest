#pragma once

#include "ArrayType.h"
#include "ThreadSafe_Map.h"
#include "GameManager.hpp"
#include "FlatbufferPacket.h"

namespace realtrick {
    
    enum class RoomState : int
    {
        INIT = 0,
        WAIT = 1000,
        READY,
        INGAME,
        EXIT
        };
        
        enum class ModeType : int
        {
            NONE = 0,
            BATTLE_ROYAL_MODE = 1000
        };
            
            
            class GameManager;
            
            class EntityBase;
            
            class Room
            {
                
            public:
                
                Room(int roomId, int roomMode) :
                _isOpen(true),
                _roomId(roomId),
                _roomMode(roomMode),
                _roomState(0),
                _maxRoomPlayer(32),
                _validID(0),
                _room_timer(0)
                {}
                
                ~Room()
                {
                    delete(_gameManager);
                    _gameManager = nullptr;
                }
                
                int getRoomId();
                
                int getRoomState();
                
                void setRoomState(int state);
                
                int getVidToPid(int validId)
                {
                    for (std::map<int, int>::const_iterator idMap = _idConvertGroup.begin(); idMap != _idConvertGroup.end(); ++idMap)
                    {
                        if (idMap->second == validId) return idMap->first;
                    }
                    return -1;
                };
                
                int getPidToVid(int pId)
                {
                    std::map<int, int>::iterator findId = _idConvertGroup.find(pId);
                    
                    if (_idConvertGroup.end() != findId) {
                        return findId->second;
                    }
                    return -1;
                };
                
                void setIdConvertGroup(int pId, int validId)
                {
                    _idConvertGroup.insert( { pId, validId } );
                };
                
                void deleteIdConvertGroup(int pId)
                {
                    std::map<int, int>::iterator findId = _idConvertGroup.find(pId);
                    
                    if (_idConvertGroup.end() != findId) {
                        _idConvertGroup.erase(findId);
                    }
                };
                
                int getRoomMode();
                
                int getMaxRoomPlayer();
                
                bool setPlayer(Session* session);
                
                Session* getPlayer(int pid);
                                
                void deletePlayer(int pid);
                
                void setLoadGamePlayer(int pid);
                
                void sendPlayerSuccessSearchGame();
                
                void refreshTimer();
                
                inline int getNextValidID();
                
                unsigned int getRoomTimer() { return _room_timer; }
                
                void initGame();
                
                void updateGame();
                
                void moveJoystick(FlatbufferPacket* packet, int pid);
                
                
            private:
                
                
                GameManager*							_gameManager;
                
                bool									_isOpen;
                
                int										_roomId;
                
                int										_roomMode;
                
                int										_roomState;
                
                int										_maxRoomPlayer;
                
//                std::map<int, realtrick::Session*>		_pSessionGroup;
                
                std::map<int, int>						_idConvertGroup;
                
                int										_validID;
                
                unsigned int                            _room_timer;
                
            };
        }