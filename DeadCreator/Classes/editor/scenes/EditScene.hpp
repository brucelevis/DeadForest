//
//  EditScene.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 6. 26..
//
//

#pragma once

#include "cocos2d.h"
#include "ImGuiLayer.h"
#include "EditorEntity.hpp"
#include "EditorType.hpp"

namespace realtrick
{
    namespace editor
    {
        
        class GMXFile;
        class GMXLayer;
        class NewFileLayer;
        class SaveAsLayer;
        class OpenLayer;
        class SaveQueryLayer;
        class PlayGameLayer;
        
        class EditScene : public ImGuiLayer
        {
            
        public:
            
            CREATE_FUNC(EditScene);
            
            static cocos2d::Scene* createScene();
            
            virtual bool init() override;
            
            void setEnableFileMenu(bool enable) { _isFileEnable = enable; }
            void setEnableEditMenu(bool enable) { _isEditEnable = enable; }
            void setEnableWindowMenu(bool enable) { _isWindowEnable = enable; }
            void setEnablePlayerMenu(bool enable) { _isPlayerEnable = enable; }
            void setEnableSaveButton(bool enable) { _enableSaveMap = enable; }
            
            void createGMXLayer(GMXFile* file);
            void createGMXLayer(const std::string& filePath);
            void closeGMXLayer();
            
            void setLayerType(LayerType type) { _layerType = static_cast<int>(type); }
            LayerType getLayerType() const { return static_cast<LayerType>(_layerType); }
            
            bool isNew();
            bool isOpen();
            bool isSave();
            bool isUndo();
            bool isRedo();
            
            void doNewButton();
            void doSaveButton();
            void doOpenButton();
            
            void changeLayerType(LayerType type);
            
            void saveFile(const std::string& filePath);
            void saveAsFile();
            
            bool isModal() const { return _isModal; }
            void enableModal(bool enable) { _isModal = enable; }
            
            void setSelectedPlayerType(PlayerType type) { _selectedPlayerType = static_cast<int>(type) - 1; }
            PlayerType getSelectedPlayerType() { return static_cast<PlayerType>(_selectedPlayerType + 1); }
            
            void playGame();
            void stopGame();
            
        private:
            
            GMXLayer* _layer = nullptr;
            NewFileLayer* _newFileLayer = nullptr;
            SaveAsLayer* _saveAsLayer = nullptr;
            OpenLayer* _openLayer = nullptr;
            SaveQueryLayer* _saveQueryLayer = nullptr;
            PlayGameLayer* _playGameLayer = nullptr;
            
            bool _showGMXLayer = false;
            bool _showNewMap = false;
            bool _showSaveAs = false;
            bool _showOpenMap = false;
            bool _showSaveQueryLayer = false;
            bool _showPlayGameLayer = false;
            
            bool _isEditMode = false;
            
            bool _enableOpenMap = true;
            bool _enableSaveMap = false;
            
            bool _isFileEnable = true;
            bool _isEditEnable = false;
            bool _isPlayerEnable = false;
            bool _isWindowEnable = false;
            bool _isModal = false;
            
            int _layerType = -1;
            int _selectedPlayerType = -1;
            
        };
        
    }
}









