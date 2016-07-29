//
//  PlayGameLayer.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 28..
//
//

#pragma once

#include "cocos2d.h"

#include "SizeProtocol.h"
#include "MainMenu3.hpp"

#include "imgui.h"
#include "imgui_internal.h"

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
            {}
            virtual ~PlayGameLayer() = default;
            
            static PlayGameLayer* create(EditScene* layer)
            {
                auto ret = new (std::nothrow) PlayGameLayer(layer);
                if ( ret && ret->init() )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            virtual bool init() override
            {
                if ( !cocos2d::Node::init() )
                    return false;
                
                return true;
            }

            void showLayer(bool& opened);
            
            void playGame();
            void closeLayer();
            void setGameStart(bool enable) { _isGameStarted = enable; }
            
        private:
            
            EditScene* _imguiLayer;
            client::DummyScene* _gameLayer = nullptr;
            
            cocos2d::ClippingRectangleNode* _clipNode = nullptr;
            cocos2d::DrawNode* _debugNode = nullptr;
            
            bool _isGameStarted = false;
            
        };
        
    }
}









