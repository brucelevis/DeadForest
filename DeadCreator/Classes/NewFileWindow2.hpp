//
//  NewFileWindow2.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 24..
//
//

#pragma once

#include "cocos2d.h"
#include "imgui.h"

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
        
        int _tileSizeXItem = 0;
        
        int _tileSizeYItem = 0;
        
        int _numOfTileX = 0;
        
        int _numOfTileY = 0;
        
        int _currentTile = 0;
        
    };
    
}











