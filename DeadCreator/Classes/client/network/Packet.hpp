//
//  Packet.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 5. 3..
//
//

#pragma once

#include <cstdio>
#include <cstring>
#include <cstdlib>

enum class PacketType : int
{
    LOGIN = 1000,
    LOGIN_SUCCESS,         //sc
    LOGIN_FAIL,            //sc
    SIGN_UP,
    DO_SEARCH_GAME,
    CANCEL_SEARCH_GAME,
    SUCCESS_SEARCH_GAME,   //sc
    FAIL_SEARCH_GAME,      //sc
    FISRT_PLAYER_INFOS,      //sc
    SUCCESS_LOAD_GAME,
    GAME_START,            //sc
    MOVE_JOYSTICK,         //sc,cs
    LOAD_GMXFILE,
};

class Packet
{
    
public:
    
    enum { HEADER_LENGTH = 8 };
    enum { MAX_BODY_LENGTH = 4096};
    
    void encode(void* body, uint32_t size, PacketType type)
    {
        char headerInSize[5] = "";
        sprintf(headerInSize, "%04d", static_cast<int>(size));
        char headerInType[5] = "";
        sprintf(headerInType, "%04d", static_cast<int>(type));
        
        _bodyLength = size;
        
        memcpy(_data, headerInSize, 4);
        memcpy(_data + 4, headerInType, 4);
        memcpy(_data + HEADER_LENGTH, body, size);
    }
    
    bool decode()
    {
        char headerInSize[5] = "";
        strncat(headerInSize, _data, 4);
        _bodyLength = static_cast<uint32_t>(atoi(headerInSize));
        
        char headerInType[5] = "";
        strncat(headerInType, _data + 4, 4);
        _type = static_cast<PacketType>(atoi(headerInType));
        
        if ( _bodyLength > MAX_BODY_LENGTH )
        {
            _bodyLength = 0;
            return false;
        }
        
        return true;
    }
    
    char* body() { return _data + HEADER_LENGTH; }
    const char* body() const { return _data + HEADER_LENGTH; }
    
    char* data() { return _data; }
    const char* data() const { return _data; }
    
    PacketType type() const { return _type; }
    uint32_t bodyLength() const { return _bodyLength; }
    uint32_t length() const { return _bodyLength + HEADER_LENGTH; }
    
private:
    
    char _data[HEADER_LENGTH + MAX_BODY_LENGTH];
    PacketType _type;
    uint32_t _bodyLength;
    
};









