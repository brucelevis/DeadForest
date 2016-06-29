//
//  EntityBase.hpp
//  TheDeadForest
//
//  Created by 남준현 on 2015. 11. 12..
//  
//

#pragma once

#include "cocos2d.h"

    class Telegram;
    class GameManager;
    
    class EntityBase : public cocos2d::Node
    {

    public:
        
        EntityBase(const EntityBase& rhs);
        
        EntityBase(GameManager* gameMgr);
        
        virtual ~EntityBase()
        {
            _gameMgr = nullptr;
        }
        
        GameManager*    getGameManager() const      { return _gameMgr; }
        
        int             getFamilyMask() const       { return _familyMask; }
        
        int             getEntityType() const       { return _entityType; }
        
        void            setEntityType(int type)     { _entityType = type; }
        
        cocos2d::Vec2   getWorldPosition() const    { return _worldPosition; }
        
        void            setWorldPosition(const cocos2d::Vec2& pos)  { _worldPosition = pos; }
        
        virtual void    visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
        
        virtual void    update(float dt) override {}
        
        virtual void    enableNormal(bool enable) {}
        
    protected:
        
        GameManager*            _gameMgr;
        
        int                     _familyMask;
        
        int                     _entityType;
        
        cocos2d::Vec2           _worldPosition;
        
    };
    








