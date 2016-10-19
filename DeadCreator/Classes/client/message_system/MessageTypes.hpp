//
//  MessageTypes.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 12..
//
//

#pragma once

#include <string>

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "JoystickEx.hpp"

namespace realtrick
{
    namespace client
    {
        
        class EntityPlayer;
        class ItemBase;
        class WeaponBase;
        class ItemSlot;
        class HumanBase;
        
        enum MessageType : int
        {
            RELOAD_WEAPON = 0,
            HITTED_BY_GUN = 1,
            HITTED_BY_AXE = 2,
            HITTED_BY_FIST = 3,
            WEAPON_READY = 4,
            REMOVE_SELF = 5,
            PLAY_SOUND = 6,
            DIE = 7,
            LOAD_GAME_PLAYER = 8,
            LOAD_GAME_COMPLETE = 9,
            MOVE_JOYSTICK_INPUT = 10,
            ATTACK_JOYSTICK_INPUT = 11,
            PUSH_ITEM_TO_INVENTORY = 12,
            PRESS_EQUIP_WEAPON_BUTTON = 13,
            PRESS_RELEASE_WEAPON_BUTTON = 14,
            PRESS_RELOAD_BUTTON = 15,
            RELOAD_COMPLETE = 16,
            BEZEL_DIRECTION_TRIGGERED = 17,
            M16A2_SHOOT = 18,
            SHOW_NEXT_FRAME = 19,
            ADD_OBJECT_TO_RENDER = 20,
            NO_HIT = 21,
            HIT = 22,
            ATTACK_BY_WEAPON = 23,
            ATTACK_BY_FIST = 24,
            DISPLAY_TEXT = 25,
            USE_ITEM_FROM_INVENTORY = 26,
            SYNC_INVENTORY_WEAPON_VIEW = 27,
            MOVE_BALANCE = 28,
            RESET_BALANCE = 29,
            PAUSE_GAME = 30,
            RESUME_GAME = 31,
        };
        
        enum HumanBehaviorType
        {
            NONE                            = 1 << 0,
            ATTACK                          = 1 << 1,
            MOVE                            = 1 << 2,
            TURN                            = 1 << 3,
        };
        
        struct ReceiverSenderDamage
        {
            HumanBase* receiver;
            HumanBase* sender;
            int damage;
        };
        
        struct MoveJoystickData
        {
            cocos2d::Ref* ref;
            cocos2d::Vec2 dir;
            JoystickEx::ClickEventType type;
        };
        
        struct AttJoystickData
        {
            cocos2d::Ref* ref;
            cocos2d::ui::Widget::TouchEventType type;
        };
        
        struct ItemAndOwner
        {
            HumanBase* owner;
            ItemBase* item;
        };
        
        struct ItemSlotData
        {
            ItemSlot* slot;
        };
        
        struct ReloadData
        {
            WeaponBase* reloadWeapon;
        };
        
        struct BezelDirectionTriggerData
        {
            cocos2d::Ref* ref;
            cocos2d::Vec2 dir;
            cocos2d::ui::Widget::TouchEventType type;
        };
        
    }
}









