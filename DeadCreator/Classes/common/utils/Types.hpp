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
        ENTITY_BASE         = 1 << 0,
        HUMAN_BASE          = 1 << 1,
        ITEM_BASE           = 1 << 2,
        WEAPON_BASE         = 1 << 3,
        BULLET_BASE         = 1 << 4,
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
		BULLET_9MM = 8,
		BULLET_SHELL = 9,
		BULLET_AXE = 10,
		ENTITY_PLAYER = 11,
		ENTITY_ZOMBIE = 12,
		ENTITY_ZOMBIE2 = 13,
		ENTITY_ZOMBIE3 = 14,
		ENTITY_FINITE = 15,
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
        
    enum class TriggerComponentType : int
    {
        CONDITION_ALWAYS,
        CONDITION_BRING,
        CONDITION_COUNTDOWN_TIMER,
        CONDITION_ELAPSED_TIME,
        CONDITION_NEVER,
        CONDITION_SWITCH,
        ACTION_DISPLAY_TEXT,
        ACTION_MOVE_LOCATION,
        ACTION_KILL_ENTITY_AT_LOCATION,
        ACTION_PLAY_SOUND_AT_LOCATION,
        ACTION_PLAY_SOUND,
        ACTION_PRESERVE_TRIGGER,
        ACTION_VICTORY,
        ACTION_DEFEAT,
        ACTION_MOVE_ENTITY,
        ACTION_PAUSE_GAME,
        ACTION_RESUME_GAME,
        ACTION_SET_COUNTDOWN_TIMER
    };
        
    struct EntityData
    {
        int id;
        PlayerType playerType;
        EntityType entityType;
        cocos2d::Vec2 position;
    };
        
    inline void addMask(int& src, int mask)                         { src |= mask; }
    inline bool isMasked(int src, int mask)                         { return ((src & mask) == mask); }
    inline void removeMask(int& src, int mask)                      { if ( isMasked(src, mask) ) src ^= mask; }
        
}
    
    
    
    
    
    
    
    
    
