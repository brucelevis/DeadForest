//
//  Types.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 28..
//
//


#pragma once

#include <array>

#include "cocos2d.h"

#define ADD_FAMILY_MASK(src, mask) addMask(src, mask)

namespace realtrick
{
    
    enum FamilyMask
    {
        ENTITY_BASE         = 1 << 0,
        HUMAN_BASE          = 1 << 1,
        ITEM_BASE           = 1 << 2,
        WEAPON_BASE         = 1 << 3,
        BULLET_BASE         = 1 << 4,
        CONSUMABLE_BASE     = 1 << 5,
    };
    
	enum EntityType : int
	{
		DEFAULT = -1,
		ITEM_AXE = 0,
		ITEM_GLOCK17 = 1,
		ITEM_M16A2 = 2,
		ITEM_M1897 = 3,
		BULLET_556MM = 4,
		BULLET_9MM = 5,
		BULLET_SHELL = 6,
		BULLET_AXE = 7,
        CONSUMABLE_BANDAGE = 8,
        CONSUMABLE_MEATCAN = 9,
		ENTITY_PLAYER = 10,
		ENTITY_ZOMBIE = 11,
		ENTITY_ZOMBIE2 = 12,
		ENTITY_ZOMBIE3 = 13,
		ENTITY_FINITE = 14,
		ENTITY_WALL = 15,
		MAX = 16
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
        
    enum class AllyStatusType : int
    {
        INVALID = -1,
        ALLY = 0,
        ENEMY = 1,
    };
        
    enum class ArithmeticalType : int
    {
        INVALID = -1,
        ADD = 0,
        SET_TO = 1,
        SUBTRACT = 2
    };
        
    enum class SwitchStatus : int
    {
        INVALID = -1,
        CLEARED = 0,
        SET = 1
    };
        
    enum class SwitchStatus2 : int
    {
        INVALID = -1,
        CLEAR = 0,
        SET = 1,
        TOGGLE = 2,
        RANDOMIZE = 3
    };
        
    enum class OrderType : int
    {
        INVALUD = -1,
        MOVE = 0,
        ATTACK = 1,
    };
        
    enum class TriggerComponentType : int
    {
        CONDITION_ALWAYS,
        CONDITION_BRING,
        CONDITION_COUNTDOWN_TIMER,
        CONDITION_ELAPSED_TIME,
        CONDITION_NEVER,
        CONDITION_SWITCH,
        ACTION_CREATE_ENTITY,
        ACTION_DISPLAY_TEXT,
        ACTION_MOVE_LOCATION,
        ACTION_KILL_ENTITY_AT_LOCATION,
        ACTION_ORDER,
        ACTION_PLAY_SOUND_AT_LOCATION,
        ACTION_PLAY_SOUND,
        ACTION_PRESERVE_TRIGGER,
        ACTION_VICTORY,
        ACTION_DEFEAT,
        ACTION_MOVE_ENTITY,
        ACTION_PAUSE_GAME,
        ACTION_RESUME_GAME,
        ACTION_SET_ALLIANCE_STATUS,
        ACTION_SET_COUNTDOWN_TIMER,
        ACTION_SET_SWITCH,
    };
        
    struct EntityData
    {
        int id;
        PlayerType playerType;
        EntityType entityType;
        cocos2d::Vec2 position;
        std::string name;
    };
        
    struct Switch
    {
        std::array<char, 100> name;
        SwitchStatus status;
        int index;
        
        Switch() = default;
        Switch(const Switch& rhs) { copyFrom(rhs); }
        Switch& operator=(const Switch& rhs)
        {
            if ( &rhs != this ) copyFrom(rhs);
            return *this;
        }
        
        void copyFrom(const Switch& rhs)
        {
            name.fill('\0');
            std::strncpy(name.data(), rhs.name.data(), 100);
            status = rhs.status;
            index = rhs.index;
        }
    };
        
    struct Switch2
    {
        std::array<char, 100> name;
        SwitchStatus2 status;
        int index;
        
        Switch2() = default;
        Switch2(const Switch2& rhs) { copyFrom(rhs); }
        Switch2& operator=(const Switch2& rhs)
        {
            if ( &rhs != this ) copyFrom(rhs);
            return *this;
        }
        
        void copyFrom(const Switch2& rhs)
        {
            name.fill('\0');
            std::strncpy(name.data(), rhs.name.data(), 100);
            status = rhs.status;
            index = rhs.index;
        }
    };
        
    inline void addMask(int& src, int mask)                         { src |= mask; }
    inline bool isMasked(int src, int mask)                         { return ((src & mask) == mask); }
    inline void removeMask(int& src, int mask)                      { if ( isMasked(src, mask) ) src ^= mask; }
        
}
    
    
    
    
    
    
    
    
    
