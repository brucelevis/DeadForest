//
//  SimulatorLayer.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 28..
//
//

#pragma once
#define _ENABLE_ATOMIC_ALIGNMENT_FIX
#include "cocos2d.h"
#include "Mat3.hpp"
#include "SizeProtocol.h"

#include "imgui.h"
#include "imgui_internal.h"

#include "GameServer.hpp"

#include "Box2D/Box2D.h"
#include "Box2D/Common/b2Draw.h"

namespace realtrick
{
	namespace client
	{
		class DummyScene;
	}

	namespace editor
	{

		class EditScene;

		class SimulatorLayer : public cocos2d::Node, public b2Draw
		{

		public:

			explicit SimulatorLayer(EditScene* layer) : _imguiLayer(layer)
			{
                client::GameServer::getInstance().connect("127.0.0.1", "4242");
            }
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
            void receiveProfileDataAndRender();

			void playGame();
			void closeLayer();
			void setGameStart(bool enable) { _isGameStarted = enable; }
            client::DummyScene* getGameLayer() const { return _gameLayer; }
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
            
		private:

			EditScene* _imguiLayer;
			client::DummyScene* _gameLayer = nullptr;

			bool _isGameStarted = false;
			realtrick::Mat3 _cameraTransform;
            
            cocos2d::Vec2 _debugOrigin;

		};

	}
}









