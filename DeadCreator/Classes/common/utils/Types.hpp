//
//  Types.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 28..
//
//


#pragma once

#include "cocos2d.h"

#define ADD_FAMILY_MASK(src, mask) addMask(src, mask)

namespace realtrick
{
    
    enum FamilyMask
    {
        ENTITY_BASE         = 0x00000000000001,
        HUMAN_BASE          = 0x00000000000002,
        ITEM_BASE           = 0x00000000000004,
        WEAPON_BASE         = 0x00000000000008,
        BULLET_BASE         = 0x00000000000010,
    };
    
    enum EntityType : int
    {
        DEFAULT = -1,
        ITEM_AXE = 0,
        ITEM_GLOCK17 = 1,
        ITEM_M16A2 = 2,
        ITEM_M1897 = 3,
        ITEM_CONSUMPTION = 4,
        ITEM_WEAPON = 5,
        ITEM_STUFF = 6,
        BULLET_556MM = 7,
        BULLET_9MM= 8,
        BULLET_SHELL = 9,
        BULLET_AXE = 10,
        ENTITY_PLAYER = 11,
        ENTITY_ZOMBIE = 12,
        ENTITY_FINITE = 13,
    };
    
    enum class PlayerType : int
    {
        INVALID = -1,
        PLAYER1 = 1,
        PLAYER2 = 2,
        PLAYER3 = 3,
        PLAYER4 = 4,
        PLAYER5 = 5,
        PLAYER6 = 6,
        PLAYER7 = 7,
        PLAYER8 = 8,
        CURRENT_PLAYER = 9,
        NEUTRAL = 12,
    };
        
    enum class TileType : int
    {
        INVALID = -1,
        DIRT = 0,
        GRASS = 1,
        WATER = 2,
        HILL = 3,
    };
        
    enum class ApproximationType : int
    {
        INVALID = -1,
        AT_LEAST = 0,
        AT_MOST = 1,
        EXACTLY = 2,
    };
        
    enum class TriggerComponentType : int
    {
        CONDITION_BRING,
        CONDITION_ALWAYS,
        CONDITION_NEVER,
        ACTION_DISPLAY_TEXT,
        ACTION_MOVE_LOCATION,
        ACTION_KILL_ENTITY_AT_LOCATION,
        ACTION_PLAY_SOUND_AT_LOCATION,
        ACTION_PLAY_SOUND,
        ACTION_PRESERVE_TRIGGER,
    };
        
    struct EntityData
    {
        int id;
        PlayerType playerType;
        EntityType entityType;
        cocos2d::Vec2 position;
    };
    
    enum class ComponentType : int
    {
        RENDERABLE,
        MAX
    };
        
    inline void addMask(int& src, int mask)                         { src |= mask; }
    inline bool isMasked(int src, int mask)                         { return ((src & mask) == mask); }
    inline void removeMask(int& src, int mask)                      { if ( isMasked(src, mask) ) src ^= mask; }
        
}
    
    
    
    
    
    
    
    
    
