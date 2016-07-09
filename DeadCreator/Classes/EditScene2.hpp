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
    class SaveAsLayer;
    
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
        void setEnableSaveButton(bool enable) { _enableSaveMap = enable; }
        
        void createGMXLayer(GMXFile* file);
        
        void setLayerType(LayerType type) { _layerType = type; }
        int getLayerType() const { return _layerType; }
    
        bool isUndo();
        bool isRedo();
        
        void doNewButton();
        void doSaveButton();
        
        void saveFile(const std::string& filePath);
        void saveAsFile();
        
        bool isModal() const { return _isModal; }
        void enableModal(bool enable) { _isModal = enable; }
        
    private:
        
        GMXLayer2* _layer = nullptr;
        
        NewFileWindow2* _newFileWindow = nullptr;
        SaveAsLayer* _saveAsLayer = nullptr;
        
        bool _showNewMap = false;
        bool _showSaveAs = false;
        
        bool _enableOpenMap = true;
        bool _enableSaveMap = false;
        
        bool _isFileEnable = true;
        bool _isEditEnable = false;
        bool _isPlayerEnable = false;
        bool _isWindowEnable = false;
        
        int _layerType = LayerType::INVALID;
        
        bool _isModal = false;
        
    };
    
}







