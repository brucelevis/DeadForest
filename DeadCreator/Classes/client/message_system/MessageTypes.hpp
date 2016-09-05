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
            WEAPON_READY = 3,
            REMOVE_SELF = 4,
            PLAY_SOUND = 5,
            DIE = 6,
            LOAD_GAME_PLAYER = 7,
            LOAD_GAME_COMPLETE = 8,
            MOVE_JOYSTICK_INPUT = 9,
            ATTACK_JOYSTICK_INPUT = 10,
            PUSH_ITEM_TO_INVENTORY = 11,
            PRESS_EQUIP_WEAPON_BUTTON = 12,
            PRESS_RELEASE_WEAPON_BUTTON = 13,
            PRESS_RELOAD_BUTTON = 14,
            RELOAD_COMPLETE = 15,
            BEZEL_DIRECTION_TRIGGERED = 16,
            M16A2_SHOOT = 17,
            SHOW_NEXT_FRAME = 18,
            ADD_OBJECT_TO_RENDER = 19,
            NO_HIT = 20,
            HIT = 21,
            ATTACK_BY_WEAPON = 22,
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
            EntityPlayer* owner;
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









