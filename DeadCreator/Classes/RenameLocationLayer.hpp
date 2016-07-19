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
    
    class GMXLayer;
    
    class RenameLocationLayer : public cocos2d::Node
    {
        
    public:
        
        explicit RenameLocationLayer(GMXLayer& layer);
        virtual ~RenameLocationLayer();
        static RenameLocationLayer* create(GMXLayer& layer);
        
        void showLayer(bool* opened);
        void closeWindow();
        
        void setInputText(const std::string& inputText);
        
    private:
        
        GMXLayer& _gmxLayer;
        
        char _locationName[20];
        ImGuiButtonFlags _okButtonFlags = ImGuiButtonFlags_Disabled;
        float _okButtonTextAlpha = 0.5f;
        bool _isPossibleRenameLocation = false;
        
    };
    
}