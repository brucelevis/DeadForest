//
//  Game.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 27..
//
//

#pragma once

#include "audio/include/AudioEngine.h"

#include "cocos2d.h"
#include "EntityPlayer.hpp"
#include "SoundSource.hpp"
#include "StringHelper.hpp"
#include "SizeProtocol.h"
#include "MessageDispatcher.hpp"
#include "MessageNode.hpp"
#include "SimpleReleasePool.hpp"
#include "Physics.hpp"

#include "SparseGraph.hpp"
#include "GraphNodeTypes.hpp"
#include "GraphEdgeTypes.hpp"
#include "PathEdge.hpp"

#include "PhysicsManager.hpp"


#define Z_ORDER_GAME_MAP    0
#define Z_ORDER_SHADOW      1
#define Z_ORDER_LIGHT       2
#define Z_ORDER_ITEMS       3
#define Z_ORDER_HUMAN       4
#define Z_ORDER_UI          10

#define BF_MULTIPLY         { GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA }
#define BF_ADDITIVE         { GL_ONE, GL_ONE }

namespace realtrick
{
    namespace editor { class SimulatorLayer; }
    
    namespace client
    {
        
        class EntityBase;
        class TriggerSystem;
        class RenderingSystem;
        class LogicStream;
        class GameResource;
        class EntityManager;
        class HumanBase;
        class UiLayer;
        class Camera2D;
		class Wall;
     

        class Game : public cocos2d::Node
        {
            
        public:

			typedef NavGraphNode							GraphNode;
			typedef NavGraphEdge							GraphEdge;
			typedef SparseGraph<GraphNode, GraphEdge>		Graph;

            Game();
            virtual ~Game();
            
            virtual bool init() override;
            bool initWithSimulator(editor::SimulatorLayer* simulator);
            
            static Game* create();
            static Game* createWithSimulator(editor::SimulatorLayer* simulator);
            static cocos2d::Scene* createScene();
            
            void clear();
            void update(float dt) override;
            
            GameResource* getGameResource() const { return _gameResource; }
            EntityManager* getEntityManager() const { return _entityManager; }
            RenderingSystem* getRenderingSysetm() const { return _renderingSystem; } 
            TriggerSystem* getTriggerSystem() const { return _triggerSystem; }
			Graph* getGraph() const { return _graph; }
            
            // entity helper
            void addEntity(EntityBase* ent, int zOrder = 0);
            void removeEntity(EntityBase* ent);
            HumanBase* getPlayerPtr() const;
            HumanBase* getPlayerPtr(PlayerType type) const;
            EntityBase* getEntityFromID(int id) const;
            
            void killEntity(EntityBase* ent);
            void teleportEntity(EntityBase* ent, const cocos2d::Rect& rect); // todo
            cocos2d::Vec2 getSuiatablePosition(const Circle& circle, const cocos2d::Rect& maximumRegion); // todo
            
            void loadGMXFileFromPath(const std::string& path);
            void loadGMXFileFromBinary(const char* binary);
            void loadGameContents(PlayerType ownPlayer);
            void loadBGM();
            
            std::vector<EntityBase*> getNeighbors(const cocos2d::Vec2& pos) const;
            std::vector<EntityBase*> getNeighborsOnMove(const cocos2d::Vec2& pos, float speed) const;
            std::vector<EntityBase*> getNeighborsOnAttack(const cocos2d::Vec2& pos, const cocos2d::Vec2& dir, float range) const;
            std::vector<EntityBase*> getNeighborsEntities(const cocos2d::Vec2& pos, const cocos2d::Rect& rect) const;

            std::vector<realtrick::Polygon> getNeighborWalls(const cocos2d::Vec2& pos) const;
            std::vector<realtrick::Polygon> getNeighborWalls(const cocos2d::Vec2& pos, float speed) const;
            std::vector<realtrick::Polygon> getNeighborWalls(const cocos2d::Vec2& pos, const cocos2d::Size screenSize) const;
            std::vector<realtrick::Polygon> getNeighborWalls(const cocos2d::Vec2& pos, const Segment& ray) const;
            std::vector<realtrick::Polygon> getNeighborSimpleWalls(const cocos2d::Vec2& pos) const;
            std::vector<realtrick::Polygon> getNeighborSimpleWalls(const cocos2d::Vec2& pos, float speed) const;
            std::vector<realtrick::Polygon> getNeighborSimpleWalls(const cocos2d::Vec2& pos, const cocos2d::Size screenSize) const;
            std::vector<realtrick::Polygon> getNeighborSimpleWalls(const cocos2d::Vec2& pos, const Segment& ray) const;
			
			std::vector<Wall*> queryWalls(const cocos2d::Vec2& pos) const;
			std::vector<Wall*> queryWalls(const cocos2d::Vec2& pos, float radius) const;
			std::vector<Wall*> queryWalls(const cocos2d::Vec2& pos, const cocos2d::Size screenSize) const;
			std::vector<Wall*> queryWalls(const cocos2d::Vec2& pos, const Segment& ray) const;


			bool isCollideSimpleWalls(const cocos2d::Vec2& pos) const;

            TileType getStepOnTileType(const cocos2d::Vec2& pos);
            
            void sendMessage(double delaySeconds, MessageNode* receiver, MessageNode* sender, MessageType type, void* extraInfo);
            
            void pushLogic(double delaySeconds, MessageType type, void* extraInfo);
            bool isPaused() const { return _isPaused; }
            void pauseGame() { _isPaused = true; }
            void resumeGame() { _isPaused = false; }
            
            void addLog(const std::string& log);
            void clearLogs();
            const std::string& getLogString() const { return _logString; }
            bool& isLogAdded() { return _isLogAdded; }
            
            Camera2D* getCamera() const { return _camera; }

			void generateIsometricGridGraph(int numOfTileX, int numOfTileY, float tileWidth, float tileHeight, int numOfDummy);

			bool isLOSOkay(cocos2d::Vec2 A, cocos2d::Vec2 B) const;
			bool isLOSOkay(cocos2d::Vec2 A, cocos2d::Vec2 B, float radius) const;
			
            void replaceVictoryScene(float delay);
            void replaceDefeatScene(float delay);
            
            bool isAllyState(PlayerType src, PlayerType dest);
            bool isSharedVisionState(PlayerType src, PlayerType dest);
            void setAllyState(PlayerType src, PlayerType dest, bool enable);
            void setSharedVisionState(PlayerType src, PlayerType dest, bool enable);
            
            bool isGameEnded() const { return _isGameEnded; }
            
            const float getElapsedTime() const { return _elapsedTime; }
            const int getCountdownTimer() const;
            void setCountdownTimer(unsigned int seconds);
            void addCountdownTimer(unsigned int seconds);
            void subtractCountdownTimer(unsigned int seconds);
            
            SwitchStatus getSwitchStatus(int index) const;
            void setSwitchStatus(int index, SwitchStatus status);
            
            PhysicsManager* getPhysicsManager() const { return _physicsMgr; }
            
        private:
            
            cocos2d::Size _winSize;
            SimpleReleasePool _releasePool; // memory release manager
            
            GameResource* _gameResource; // map data
            EntityManager* _entityManager; // entitiy manager
            TriggerSystem* _triggerSystem; // trigger system
            RenderingSystem* _renderingSystem; // rendering system
            MessageDispatcher* _messenger; // message system
            UiLayer* _uiLayer; // ui layer
            Camera2D* _camera; // camera
            LogicStream* _logicStream; // stream mediator
			Graph* _graph; // graph
            
			PhysicsManager* _physicsMgr;

            bool _isPaused;
            float _elapsedTime;
            int _bgmID;
            
            // stored logs
            std::vector<std::pair<std::string, int>> _logs;
            std::string _logString;
            bool _isLogAdded = false;
      
            bool _isGameEnded = false;

            std::vector<cocos2d::Rect> _cellAABBs;
			cocos2d::Vector<Wall*> _walls;
            
        };
        
    }
}









