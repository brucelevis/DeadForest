//
//  SimulatorLayer.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 28..
//
//

#pragma once

#include "cocos2d.h"
#include "Mat3.hpp"
#include "SizeProtocol.h"

#include "imguix/imgui.h"
#include "imguix/imgui_internal.h"
#include "tcp_session/TCPSession.hpp"
#include "simple_profiler/profiling_schema_generated.h"

#include "Box2D/Box2D.h"
#include "Box2D/common/b2Draw.h"

namespace realtrick
{
    namespace client
    {
        class DummyScene;
        class GoalBase;
    }
    
	namespace editor
	{
    
        struct ProfileStatus
        {
            int fps;
            long drawCalls;
            long numOfVertices;
            
            ProfileStatus() : fps(0), drawCalls(0), numOfVertices(0) {}
        };
        
        struct ProfileCPU
        {
            std::string name;
            long avgTime;
            long minTime;
            long maxTime;
            long calls;
            float usage;
            std::vector<ProfileCPU> children;
            
            ProfileCPU() : name(""), avgTime(0), minTime(10000000), maxTime(0), calls(0), usage(0.0f), children() {}
        };
        
		class EditScene;
        
		class SimulatorLayer : public cocos2d::Node, public b2Draw
		{

        public:
            
            static const ImVec4 BLACK;
            static const ImVec4 GRAYLISH_GREEN;
            static const ImVec4 GRAYLISH_RED;
            static const ImVec4 GRAYLISH_BLUE;
            
		public:

            explicit SimulatorLayer(EditScene* layer);
			virtual ~SimulatorLayer() = default;

			static SimulatorLayer* create(EditScene* layer)
			{
				auto ret = new (std::nothrow) SimulatorLayer(layer);
				if (ret && ret->init())
				{
					ret->autorelease();
					return ret;
				}
				CC_SAFE_DELETE(ret);
				return nullptr;
			}

			virtual bool init() override
			{
				if (!cocos2d::Node::init())
					return false;

				return true;
			}

			void showLayer(bool& opened);

			void playGame();
			void closeLayer();
			void setGameStart(bool enable) { _isGameStarted = enable; }
            client::DummyScene* getGameLayer() const { return _gameLayer; }
            
        private:
 
            cocos2d::Vec2 worldToLocal(const cocos2d::Vec2& p);
            cocos2d::Vec2 worldToLocal(const cocos2d::Vec2& origin, const cocos2d::Vec2& p);
            bool isNeighborPoint(const cocos2d::Vec2& p);
            
            // override funcs to draw box2d debug node
            virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
            virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
            virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;
            virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;
            virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
            void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
            virtual void DrawTransform(const b2Transform& xf) override {};
            
            // Vec2 <-> b2Vec convert helpers
            template<typename Point> static inline cocos2d::Vec2 toVec(const Point& p) { return cocos2d::Vec2(p.x, p.y); }
            template<typename Point> static inline b2Vec2 tob2Vec(const Point& p) { return b2Vec2(p.x, p.y); }
            
            void fillProfileCPUDatas(std::vector<ProfileCPU>& parent, const realtrick::profiler::Element* elem);
            void drawProfileCPUs(const ProfileCPU& data);
            
            void drawGoalDatas(int blanks, realtrick::client::GoalBase* goal);
            
		private:

			EditScene* _imguiLayer;
			client::DummyScene* _gameLayer = nullptr;

			bool _isGameStarted = false;
			realtrick::Mat3 _cameraTransform;
            
            cocos2d::Vec2 _debugOrigin;
            realtrick::network::TCPSession _tcpSession;
            
            ImVec2 _profileRenderOrigin;
            std::list<ProfileStatus> _profileStatus;
            std::list<ProfileCPU> _profileCPU;
            
            int _lines = 0;
            
            bool _isPlayerInfo = true;
            bool _isGridOn = false;
            bool _isLocationViewOn = false;
            bool _isGraphNodeViewOn = false;
            
            bool _isPhysicsShape = false;
            bool _isPhysicsAABB = false;
            
            bool _isNavGraphOn = false;
            bool _isGoalDescOn = false;
            
		};

	}
}









