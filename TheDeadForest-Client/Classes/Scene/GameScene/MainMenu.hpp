//
//  MainMenu.hpp
//  TheDeadForest
//
//  Created by 남준현 on 2015. 11. 29..
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace realtrick
{
    
    class MainMenu : public cocos2d::Layer
    {
        
    public:
        
        cocos2d::ui::Text*              queueText;
        cocos2d::ui::Text*              queueWaitText;
        cocos2d::ui::Button*            queueBackBt;
        int                             currQueueUser;
        
    public:
        
        MainMenu();
        
        virtual ~MainMenu();
        
        static cocos2d::Scene* createScene();
        
        virtual bool init() override;
        
        CREATE_FUNC(MainMenu);
        
        virtual void update(float dt) override;
        
    private:
        
        cocos2d::Size                   _winSize;
        
        cocos2d::Sprite*                _title;
        
        std::vector<cocos2d::Sprite*>   _dots;
        
        cocos2d::ui::Button*            _singleBt;
        
        cocos2d::ui::Button*            _multiBt;
        
        cocos2d::ui::Button*            _exitBt;
        
        cocos2d::ui::Button*            _quickStartBt;
        
        cocos2d::ui::Button*            _backBt;
        
        cocos2d::ui::Text*              _connectInfo;
        
        cocos2d::ui::Text*              _version;
        
        cocos2d::ui::Text*              _idText;
        
        std::string                     _userID;
        
        bool                            _isRegist;
        
        bool                            _isNetworkReady;
        
        float                           _networkAccumulateTime;
        
        bool                            _isLoading;
        
        bool                            _isSingleReady;
        
        float                           _singleAccumulateTime;
        
        bool                            _isConnect;
        
        cocos2d::Node*                  _packetRecvLoop;
        
        cocos2d::Sprite*                _node[5];
        
    public:
        
        void _prepareSingleMode();
        
        void _prepareMultiMode();
        
        void _prepareMultiModeStart();
        
        void _prepareMultiModeFail();
        
        void _enableLoading(bool enable);
        
        void _enableButtons(bool enable);
        
        void _hideButtons();
        
        void _showButtons();
        
        void _disconnectFromServer();
        
    };
    
}
