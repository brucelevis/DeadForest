//
//  SaveAsLayer.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 9..
//
//

#pragma once

#include "cocos2d.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace realtrick
{
    
    class EditScene;
    
    class SaveAsLayer : public cocos2d::Node
    {
        
    public:
        
        explicit SaveAsLayer(EditScene* layer);
        virtual ~SaveAsLayer();
        static SaveAsLayer* create(EditScene* layer);
        
        void showLayer(bool* opened);
        void closeWindow();
        
        void checkIsSaveFile();
        
    private:
        
        EditScene* _imguiLayer;
        
        char _filePath[256];
        ImGuiButtonFlags _saveButtonFlags = ImGuiButtonFlags_Disabled;
        float _saveButtonTextAlpha = 0.5f;
        bool _isPossibleSave = false;
        
    };
    
}









