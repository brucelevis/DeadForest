//
//  WeaponStatus.hpp
//  TheDeadForest
//
//  Created by mac on 2016. 5. 14..
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GameManager.hpp"
#include "EntityType.hpp"

namespace realtrick
{
    namespace client
    {
        
        class WeaponBase;
        class GameManager;
        
        class WeaponStatus : public cocos2d::Node
        {
            
        public:
            
            explicit WeaponStatus(GameManager* mgr);
            virtual ~WeaponStatus() = default;
            
            virtual bool init() override;
            static WeaponStatus* create(GameManager* mgr);
            
            void setRemainBullet(int num)
            {
                _remainBulletText->setString(GameManager::_to_string(num));
                _numOfRemainBullet = num;
            }
            
            void setEntryBullet(WeaponBase* weapon);
            void setEntryBullet(EntityType bulletType);
            
            int getRemainBullet() const { return _numOfRemainBullet; }
            void setReloadCallBack(const cocos2d::ui::Widget::ccWidgetTouchCallback& callback) { _reloadButton->addTouchEventListener(callback); }
            void setWeaponStatus(WeaponBase* weapon);
            
            void disableButton();
            void enableButton();
            
        private:
            
            void _setEntryBulletNumber(int num)
            {
                _entryBulletText->setString(GameManager::_to_string(num));
                _numOfEntryBullet = num;
            }
            
        private:
            
            GameManager* _gameMgr;
            cocos2d::ui::Button* _reloadButton;
            cocos2d::ui::Text* _remainBulletText;
            cocos2d::ui::Text* _entryBulletText;
            WeaponBase* _displayedWeapon;
            
            int _numOfRemainBullet;
            int _numOfEntryBullet;
            
        };
        
    }
}









