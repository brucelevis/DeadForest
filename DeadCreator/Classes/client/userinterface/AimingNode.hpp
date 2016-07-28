//
//  AimingNode.hpp
//  TheDeadForest
//
//  Created by mac on 2016. 5. 16..
//
//

#pragma once

#include "cocos2d.h"

namespace realtrick
{
    namespace client
    {
        
        class GameManager;
        
        class AimingNode : public cocos2d::Node
        {
            
        public:
            
            explicit AimingNode(GameManager* mgr);
            virtual ~AimingNode() = default;
            
            virtual bool init() override;
            static AimingNode* create(GameManager* mgr);
            
            void runHitAction();
            void showOutter();
            void hideOutter();
            void showInner();
            void hideInner();
            void setRange(float range);
            
        private:
            
            GameManager* _gameMgr;
            cocos2d::Sprite* _crossHair;
            cocos2d::Sprite* _outter;
            cocos2d::Sprite* _inner;
            
            float _range;
            bool _isOutterOn;
            bool _isInnerOn;
            
        };
        
    }
}









