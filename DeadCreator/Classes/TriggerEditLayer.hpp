//
//  TriggerEditLayer.hpp
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
    
    class TriggerEditLayer : public cocos2d::Node
    {
        
    public:
        
        explicit TriggerEditLayer(GMXLayer2& layer);
        virtual ~TriggerEditLayer();
        static TriggerEditLayer* create(GMXLayer2& layer);
        
        void showLayer(bool* opened);
        void closeWindow();
        
    private:
        
        GMXLayer2& _gmxLayer;
        
    };
    
}