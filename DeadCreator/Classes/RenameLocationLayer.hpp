//
//  RenameLocationLayer.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 16..
//
//

#pragma once

#include "cocos2d.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace realtrick
{
    
    class GMXLayer2;
    
    class RenameLocationLayer : public cocos2d::Node
    {
        
    public:
        
        explicit RenameLocationLayer(GMXLayer2& layer);
        virtual ~RenameLocationLayer();
        static RenameLocationLayer* create(GMXLayer2& layer);
        
        void showLayer(bool* opened);
        void closeWindow();
        
        void setInputText(const std::string& inputText);
        
    private:
        
        GMXLayer2& _gmxLayer;
        
        char _locationName[20];
        ImGuiButtonFlags _okButtonFlags = ImGuiButtonFlags_Disabled;
        float _okButtonTextAlpha = 0.5f;
        bool _isPossibleRenameLocation = false;
        
    };
    
}