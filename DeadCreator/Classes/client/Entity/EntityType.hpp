//
//  EntityType.hpp
//  TheDeadForest
//
//  Created by 남준현 on 2015. 11. 28..
//
//


#pragma once

#define ADD_FAMILY_MASK(src, mask) addMask(src, mask)

namespace realtrick
{
    
    enum FamilyMask
    {
        ENTITY_BASE         = 0x00000000000001,
        DYNAMIC_ENTITY      = 0x00000000000002,
        ITEM_BASE           = 0x00000000000004,
        WEAPON_BASE         = 0x00000000000008,
        CONSUMPTION         = 0x00000000000010,
        STUFF               = 0x00000000000020,
        BULLET_BASE         = 0x00000000000040,
        FINITE_BASE         = 0x00000000000080
    };
    
    enum EntityType : int
    {
        DEFAULT = -1,
        ITEM_AXE = 0,
        ITEM_GLOCK17 = 1,
        ITEM_M16A2 = 2,
        ITEM_M1897 = 3,
        ITEM_CONSUMPTION,
        ITEM_WEAPON,
        ITEM_STUFF,
        BULLET_556MM,
        BULLET_9MM,
        BULLET_SHELL,
        BULLET_AXE,
        ENTITY_HUMAN,
        ENTITY_BLOOD,
        ENTITY_EMPTY_CARTRIDGE,
        ENTITY_RAIN_DROP
    };
    
    enum class PlayerType : int
    {
        INVALID = -1,
        PLAYER1 = 0,
        PLAYER2,
        PLAYER3,
        PLAYER4,
        PLAYER5,
        PLAYER6,
        PLAYER7,
        PLAYER8,
        CURRENT_PLAYER,
        NEUTRAL = 12,
    };
    
    inline void addMask(int& src, int mask)                         { src |= mask; }
    inline bool isMasked(int src, int mask)                         { return ((src & mask) == mask); }
    inline void removeMask(int& src, int mask)                      { if ( isMasked(src, mask) ) src ^= mask; }
    
    enum TileType : short
    {
        _1,
        _2,
        _3,
        _4,
        _12,
        _13,
        _14,
        _23,
        _24,
        _34,
        _123,
        _124,
        _134,
        _234,
        _1234
    };
    
}










