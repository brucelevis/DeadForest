//
//  RenderingSystem.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 31..
//
//

#pragma once

#include "cocos2d.h"

namespace realtrick
{
    namespace client
    {
     
        class Game;
        class GameResource;
        class Camera2D;
        class TileImage;
        class Terrain;
        class EntityBase;
        class UiLayer;
        
        class RenderingSystem : public cocos2d::Node
        {
            
        public:
            
            explicit RenderingSystem(Game* game);
            virtual ~RenderingSystem();
            
            static RenderingSystem* create(Game* game, GameResource* res);
            bool init(GameResource* res);
            
            void render(Camera2D* camera);
            
            void updateChunk(Camera2D* camera);
            void setZoom(float r) { _zoomScale = r; }
            cocos2d::Vec2 getZoomScale() const
            {
                return cocos2d::Vec2(_zoomScale * _gameScreenScale.x, _zoomScale * _gameScreenScale.y);
            }
            void cameraTransform(Camera2D* camera);
            
            cocos2d::Vec2 getGameScreenScale() const { return _gameScreenScale; }
            
            void addEntity(EntityBase* entity, int zOrder = 0);
            void removeEntity(EntityBase* entity);
            void addUINode(cocos2d::Node* node);
            
        private:
            
            Game* _game;
            GameResource* _gameResource;
            
            cocos2d::Node* _renderingNode;
            
            float _zoomScale;
            cocos2d::Vec2 _gameScreenScale;
            
            Terrain* _terrain;
            
            cocos2d::RenderTexture* _fbo;
            
        };
        
    }
}









