//
//  AimingSystem.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 5. 16..
//
//

#pragma once

namespace realtrick
{
    namespace client
    {
        
        class GameManager;
        class EntityHuman;
        class EntityBase;
        
        class AimingSystem
        {
            
        public:
            
            AimingSystem() = delete;
            explicit AimingSystem(GameManager* mgr, EntityHuman* owner);
            virtual ~AimingSystem() = default;
            
            bool isEnableSystem() const { return _isEnableSystem; }
            void enableSystem(bool enable) { _isEnableSystem = enable; }
            bool isHitableEntity() const { return _isHitableEntity; }
            EntityBase* getClosestHiableEntity() const { return _closestHitableEntity; }
            
            void update();
            
        private:
            
            GameManager* _gameMgr;
            EntityHuman* _owner;
            EntityBase* _closestHitableEntity;
            
            bool _isEnableSystem;
            bool _isHitableEntity;
            
        };
        
    }
}









