//
//  PlayGameLayer.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 28..
//
//

#pragma once

#include "cocos2d.h"
#include "Mat3.hpp"
#include "SizeProtocol.h"

#include "imgui.h"
#include "imgui_internal.h"

#include "GameServer.hpp"


namespace realtrick
{
	namespace client
	{
		class DummyScene;
	}

	namespace editor
	{

		class EditScene;

		class PlayGameLayer : public cocos2d::Node
		{

		public:

			explicit PlayGameLayer(EditScene* layer) : _imguiLayer(layer)
			{
                client::GameServer::getInstance().connect("127.0.0.1", "4242");
            }
			virtual ~PlayGameLayer() = default;

			static PlayGameLayer* create(EditScene* layer)
			{
				auto ret = new (std::nothrow) PlayGameLayer(layer);
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
            void receiveProfileData();

			void playGame();
			void closeLayer();
			void setGameStart(bool enable) { _isGameStarted = enable; }

			cocos2d::Vec2 worldToLocal(const cocos2d::Vec2& p);
			cocos2d::Vec2 worldToLocal(const cocos2d::Vec2& origin, const cocos2d::Vec2& p);

		private:

			EditScene* _imguiLayer;
			client::DummyScene* _gameLayer = nullptr;

			bool _isGameStarted = false;
			realtrick::Mat3 _cameraTransform;

		};

	}
}









