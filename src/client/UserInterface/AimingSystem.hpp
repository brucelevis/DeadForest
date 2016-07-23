//
//  AimingSystem.hpp
//  TheDeadForest
//
//  Created by mac on 2016. 5. 16..
//
//

#pragma once

namespace realtrick
{
    
    class GameManager;
    class EntityHuman;
    class EntityBase;
    
    class AimingSystem
    {
        
    public:
        
        AimingSystem() = delete;
        
        AimingSystem(GameManager* mgr, EntityHuman* owner);
        
        virtual ~AimingSystem() = default;
        
        bool isEnableSystem() const { return _isEnableSystem; }
        
        void enableSystem(bool enable) { _isEnableSystem = enable; }
        
        bool isHitableEntity() const { return _isHitableEntity; }
        
        EntityBase* getClosestHiableEntity() const { return _closestHitableEntity; }
        
        void update();
        
    private:
        
        GameManager* _gameMgr;
        
        EntityHuman* _owner;
        
        bool _isEnableSystem;
        
        bool _isHitableEntity;
        
        EntityBase* _closestHitableEntity;
        
    };
}