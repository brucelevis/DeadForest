//
//  OpenLayer.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 10..
//
//

#pragma once

#include "cocos2d.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace realtrick
{
    
    class EditScene2;
    
    class OpenLayer : public cocos2d::Node
    {
        
    public:
        
        explicit OpenLayer(EditScene2* layer);
        virtual ~OpenLayer();
        static OpenLayer* create(EditScene2* layer);
        
        void showLayer(bool* opened);
        void closeWindow();
        
        void checkIsOpenFile();
        
    private:
        
        EditScene2* _imguiLayer;
        
        char _filePath[256];
        ImGuiButtonFlags _openButtonFlags = ImGuiButtonFlags_Disabled;
        float _openButtonTextAlpha = 0.5f;
        bool _isPossibleOpen = false;
        
    };
    
}