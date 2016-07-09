//
//  NewFileWindow2.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 24..
//
//

#pragma once

#include "cocos2d.h"
#include "ImGuiLayer.h"

namespace realtrick
{
    
    class EditScene2;
    
    class NewFileWindow2 : public cocos2d::Node
    {
        
    public:
        
        explicit NewFileWindow2(EditScene2* layer);
        
        virtual ~NewFileWindow2();
        
        static NewFileWindow2* create(EditScene2* layer);
        
        void showNewFileWindow(bool* opened);
        
    private:
        
        void closeWindow();
        
    private:
        
        EditScene2* _imguiLayer;
        
        cocos2d::Size _layerSize;
        cocos2d::Vec2 _layerPosition;
        cocos2d::Rect _boundingBoxPadding;

        int _tileSizeXItem = 0;
        
        int _tileSizeYItem = 0;
        
        int _numOfTileX = 0;
        
        int _numOfTileY = 0;
        
        int _currentTile = 0;
        
    };
    
}











