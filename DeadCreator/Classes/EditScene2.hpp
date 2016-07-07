//
//  EditScene2.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 26..
//
//

#pragma once

#include "cocos2d.h"
#include "ImGuiLayer.h"

namespace realtrick
{
    
    class GMXFile;
    class GMXLayer2;
    class NewFileWindow2;
    
    enum LayerType
    {
        INVALID = -1,
        TILE ,
        ENTITY,
        DOODAD,
        LOCATION,
    };
    
    class EditScene2 : public ImGuiLayer
    {
        
    public:
        
        CREATE_FUNC(EditScene2);
        
        static cocos2d::Scene* createScene();
        
        virtual bool init() override;
        
        void setEnableFileMenu(bool enable) { _isFileEnable = enable; }
        
        void setEnableEditMenu(bool enable) { _isEditEnable = enable; }
        
        void setEnableWindowMenu(bool enable) { _isWindowEnable = enable; }
        
        void setEnablePlayerMenu(bool enable) { _isPlayerEnable = enable; }
        
        void createGMXLayer(GMXFile* file);
        
        void doNewButton();
        
        void revertNewButton();
        
        void setLayerType(LayerType type) { _layerType = type; }
        
        int getLayerType() const { return _layerType; }
        
    private:
        
        GMXLayer2* _layer = nullptr;
        
        NewFileWindow2* _newFileWindow = nullptr;
        
        bool _showNewMap = false;
        
        bool _isFileEnable = true;
        bool _enableNewMap = true;
        bool _enableOpenMap = true;
        bool _enableSaveMap = false;
        
        
        bool _isEditEnable = false;
        
        bool _isPlayerEnable = false;
        
        bool _isWindowEnable = false;
        
        int _layerType = LayerType::INVALID;
        
    };
    
}







