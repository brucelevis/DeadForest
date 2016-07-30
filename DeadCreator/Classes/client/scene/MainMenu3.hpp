//
//  MainMenu3.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 5. 1..
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"


#define MENU_POSITION_LEFT_HIDE Vec2(_winSize.width * 0.5f - 500.0f, _winSize.height* 0.35f)
#define MENU_POSITION_1 Vec2(_winSize.width * 0.5f - 400.0f, _winSize.height* 0.35f)
#define MENU_POSITION_2 Vec2(_winSize.width * 0.5f - 200.0f, _winSize.height* 0.35f)
#define MENU_POSITION_3 Vec2(_winSize.width * 0.5f, _winSize.height* 0.35f)
#define MENU_POSITION_4 Vec2(_winSize.width * 0.5f + 200.0f, _winSize.height* 0.35f)
#define MENU_POSITION_5 Vec2(_winSize.width * 0.5f + 400.0f, _winSize.height* 0.35f)
#define MENU_POSITION_RIGHT_HIDE Vec2(_winSize.width * 0.5f + 500.0f, _winSize.height* 0.35f)

namespace realtrick
{
    namespace client
    {
        
        class MainMenu3 : public cocos2d::Layer
        {
            
        public:
            
            MainMenu3();
            virtual ~MainMenu3();
            
            virtual bool init() override;
            static MainMenu3* create();
            static cocos2d::Scene* createScene();
            
            virtual void update(float dt) override;
            
        private:
            
            void _showMainMenu(float delay);
            void _showPlayMenu(float delay);
            void _showSingleMenu(float delay);
            void _showMultiMenu(float delay);
            void _showCustomMenu(float delay);
            void _showMultiSearchMenu(float delay);
            void _showCustomSearchMenu(float delay);
            void _hideMainMenu(float delay);
            void _hidePlayMenu(float delay);
            void _hideSingleMenu(float delay);
            void _hideMultiMenu(float delay);
            void _hideCustomMenu(float delay);
            void _hideMultiSearchMenu(float delay);
            void _hideCustomSearchMenu(float delay);
            void _hideMainMenuAndShowPlayMenu(float delay);
            void _hidePlayMenuAndShowMainMenu(float delay);
            void _hidePlayMenuAndShowSingleMenu(float delay);
            void _hidePlayMenuAndShowMultiMenu(float delay);
            void _hidePlayMenuAndShowCustomMenu(float delay);
            void _hideMultiMenuAndShowPlayMenu(float delay);
            void _hideMultiMenuAndShowMultiSearchMenu(float delay);
            void _hideCustomMenuAndShowPlayMenu(float delay);
            void _hideCustomMenuAndShowCustomSearchMenu(float delay);
            void _hideMultiSearchMenuAndShowMultiMenu(float delay);
            void _hideCustomSearchMenuAndShowCustomMenu(float delay);
            
        private:
            
            cocos2d::Size _winSize;
            cocos2d::Sprite* _gameTitle;
            cocos2d::Label* _userName;
            cocos2d::DrawNode* _connectState;
            cocos2d::Menu* _menuHolder;
            
            int _roomID;
            
            // main menu
            cocos2d::ui::Button* _playButton;
            cocos2d::ui::Button* _optionButton;
            cocos2d::ui::Button* _exitButton;
            
            // play menu
            cocos2d::ui::Button* _singleButton;
            cocos2d::ui::Button* _multiButton;
            cocos2d::ui::Button* _customButton;
            cocos2d::ui::Button* _backButton_play;
            
            // multi menu
            cocos2d::ClippingRectangleNode* _clipRectNode_multi;
            cocos2d::Sprite* _chapter1;
            cocos2d::Sprite* _chapter2;
            cocos2d::Sprite* _chapter3;
            cocos2d::Sprite* _chapter4;
            cocos2d::Sprite* _chapter5;
            cocos2d::ui::Text* _infoText_multi;
            cocos2d::ui::Button* _leftButton_multi;
            cocos2d::ui::Button* _rightButton_multi;
            cocos2d::ui::Button* _searchButton_multi;
            cocos2d::ui::Button* _backButton_multi;
            
            bool _isButtonMoving_multi;
            int _menuCenterIndex_multi;
            
            // custom menu
            cocos2d::ClippingRectangleNode* _clipRectNode_custom;
            cocos2d::Sprite* _battleRoyalMode;
            cocos2d::Sprite* _survivalMode;
            cocos2d::Sprite* _teamSurvivalMode;
            cocos2d::ui::Text* _infoText_custom;
            cocos2d::ui::Button* _leftButton_custom;
            cocos2d::ui::Button* _rightButton_custom;
            cocos2d::ui::Button* _searchButton_custom;
            cocos2d::ui::Button* _backButton_custom;
            
            bool _isButtonMoving_custom;
            int _menuCenterIndex_custom;
            
            // multi search menu
            std::vector<cocos2d::Sprite*> _progressDots_multi;
            cocos2d::ui::Button* _cancelButton_multiSearch;
            
            // custom search menu
            std::vector<cocos2d::Sprite*> _progressDots_custom;
            cocos2d::ui::Button* _cancelButton_customSearch;
            
        };
        
    }
}









