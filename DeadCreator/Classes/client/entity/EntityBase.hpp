//
//  EntityBase.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 12..
//
//

#pragma once

#include "cocos2d.h"
#include "Infos.hpp"
#include "MessageNode.hpp"
#include "MessageDispatcher.hpp"

namespace realtrick
{
    namespace client
    {
        
        class Telegram;
        class Game;
        
        class EntityBase : public cocos2d::Node, public MessageNode
        {
            
        public:
            
            explicit EntityBase(Game* game);
            virtual ~EntityBase() { _game = nullptr; }
            EntityBase(const EntityBase& rhs);
            
            Game* getGame() const           { return _game; }
            int getFamilyMask() const       { return _familyMask; }
            
            int getEntityType() const       { return _entityType; }
            void setEntityType(int type)     { _entityType = type; }
            
            cocos2d::Vec2 getWorldPosition() const    { return _worldPosition; }
            virtual void setWorldPosition(const cocos2d::Vec2& pos)  { _worldPosition = pos; }
            
            void setPlayerType(PlayerType type) { _playerType = type; }
            PlayerType getPlayerType() const { return _playerType; }
            
            void setForce(Force force) { _force = force; }
            Force getForce() const { return _force; }
            
            float getBoundingRadius() const { return _boundingRadius; }
            void setBoundingRadius(float r) { _boundingRadius = r; }
            
            virtual bool handleMessage(const Telegram& msg) override { return false; }
            virtual void update(float dt) override {}
            virtual void enableNormal(bool enable) {}
            
        protected:
            
            Game*                   _game;
            int                     _familyMask;
            int                     _entityType;
            cocos2d::Vec2           _worldPosition;
            PlayerType              _playerType;
            Force                   _force;
            float                   _boundingRadius;
            
        };
        
    }
}









