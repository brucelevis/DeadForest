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
        class GameObject;
        class UiLayer;
        
        class RenderingSystem : public cocos2d::Node
        {
            
        public:
            
            explicit RenderingSystem(Game* game);
            virtual ~RenderingSystem();
            
            static RenderingSystem* create(Game* game, GameResource* res);
            bool init(GameResource* res);
            
            virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
            
            cocos2d::Vec2 getCameraPosition() const;
            void setCameraPosition(const cocos2d::Vec2& pos);
            
            void updateChunk();
            void setZoom(float r) { _zoomScale = r; _rootNode->setScale(r); }
            float getZoomScale() const { return _zoomScale; }
            
            void addEntity(GameObject* entity, int zOrder);
            
        private:
            
            Game* _game;
            GameResource* _gameResource;
            
            cocos2d::ClippingRectangleNode* _clipNode;
            cocos2d::Node* _rootNode;
            
            float _zoomScale;
            
            Camera2D* _camera;
            Terrain* _terrain;
            UiLayer* _uiLayer;
            
        };
        
    }
}









