//
//  InputReload.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 4..
//
//

#pragma once

#include "cocos2d.h"

#include "InputCommandBase.hpp"
#include "EntityPlayer.hpp"
#include "WeaponBase.hpp"

namespace realtrick
{
    namespace client
    {
        
        class InputReload : public InputCommandBase
        {
            
        public:
            
            InputReload(HumanBase* player) :
            _player(player)
            {}
            virtual ~InputReload() = default;
            
            virtual void execute() override
            {
                if ( _player->isInventoryOpened() || !_player->isAlive() ) return ;
                
                WeaponBase* equipedWeapon = _player->getEquipedWeapon();
                if ( equipedWeapon && equipedWeapon->getEntityType() != EntityType::ITEM_AXE )
                {
                    _player->reload();
                }
            }
            
        private:
            
            HumanBase* _player;
            
        };
        
    }
}









