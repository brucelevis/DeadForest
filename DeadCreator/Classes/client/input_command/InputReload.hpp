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
            
            InputReload(EntityPlayer* player) :
            _player(player)
            {
                
            }
            virtual ~InputReload() = default;
            
            virtual void execute() override
            {
                if ( !_player->isInventoryOpened() )
                {
                    WeaponBase* equipedWeapon = _player->getEquipedWeapon();
                    
                    if ( equipedWeapon != nullptr &&
                        equipedWeapon->getEntityType() != EntityType::ITEM_AXE )
                    {
                        _player->reload();
                    }
                }
            }
            
        private:
            
            EntityPlayer* _player;
            
        };
        
    }
}









