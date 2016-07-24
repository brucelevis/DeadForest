//
//  Camera2D.hpp
//  TheDeadForest
//
//  Created by mac on 2016. 2. 14..
//
//

#pragma once

#include "cocos2d.h"
#include "EntityBase.hpp"

namespace realtrick
{
    class GameManager;
    
    class Camera2D : public cocos2d::Node
    {
        
    public:
        
        static Camera2D* create(GameManager* gmr);
        
        void setCameraPos(const cocos2d::Vec2& pos) { _cameraPosition = pos; }
        
        cocos2d::Vec2 getCameraPos() const { return _cameraPosition; }
        
        void pushVibration(const cocos2d::Vec2& offset) { _vibrationQueue.push_back(offset); }
        
        cocos2d::Vec2 popVibration()
        {
            cocos2d::Vec2 ret = _vibrationQueue.front();
            _vibrationQueue.pop_front();
            return ret;
        }
        
    private:
        
        cocos2d::Size                       _worldSize;
        
        GameManager*                        _gameMgr;
        
        cocos2d::Vec2                       _cameraPosition;
        
        std::deque<cocos2d::Vec2>           _vibrationQueue;
        
    private:
        
        explicit Camera2D(GameManager* mgr);
        
        virtual ~Camera2D();
        
        virtual bool init() override;
        
    };
    
} // realtrick








