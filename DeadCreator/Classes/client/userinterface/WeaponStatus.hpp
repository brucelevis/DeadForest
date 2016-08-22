//
//  WeaponStatus.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 5. 14..
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Game.hpp"
#include "Types.hpp"

namespace realtrick
{
    namespace client
    {
        
        class WeaponBase;
        class Game;
        
        class WeaponStatus : public cocos2d::Node
        {
            
        public:
            
            explicit WeaponStatus(Game* game);
            virtual ~WeaponStatus() = default;
            
            virtual bool init() override;
            static WeaponStatus* create(Game* game);
            
            void setRemainBullet(int num)
            {
                _remainBulletText->setString(_to_string(num));
                _numOfRemainBullet = num;
            }
            
            void setEntryBullet(WeaponBase* weapon);
            void setEntryBullet(realtrick::EntityType bulletType);
            
            int getRemainBullet() const { return _numOfRemainBullet; }
            void setReloadCallBack(const cocos2d::ui::Widget::ccWidgetTouchCallback& callback) { _reloadButton->addTouchEventListener(callback); }
            void setWeaponStatus(WeaponBase* weapon);
            
            void disableButton();
            void enableButton();
            
        private:
            
            void _setEntryBulletNumber(int num)
            {
                _entryBulletText->setString(_to_string(num));
                _numOfEntryBullet = num;
            }
            
        private:
            
            Game* _game;
            cocos2d::ui::Button* _reloadButton;
            cocos2d::ui::Text* _remainBulletText;
            cocos2d::ui::Text* _entryBulletText;
            WeaponBase* _displayedWeapon;
            
            int _numOfRemainBullet;
            int _numOfEntryBullet;
            
        };
        
    }
}









