//
//  GMXLayer.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 6. 28..
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "GMXFile.hpp"
#include "EditorCellSpacePartition.hpp"
#include "TileImage.hpp"
#include "Tileset.hpp"
#include "CommandQueue.hpp"
#include "SizeProtocol.h"
#include "Infos.hpp"
#include "Types.hpp"

namespace realtrick
{
    namespace editor
    {
        
        class PaletteLayer;
        class NavigatorLayer;
        class TriggerEditLayer;
        class EditScene;
        class CommandBase;
        class AddEntityToolCommand;
        class RemoveEntityToolCommand;
        class ModiftStartingPointCommand;
        class TileToolCommand;
        class EditorEntity;
        class LocationNode;
        class RenameLocationLayer;
        class EditorEntity;
        class GameTrigger;
        class HistoryLayer;
        class ForceSettingLayer;
        class PropertyEditLayer;
        
        class GMXLayer : public cocos2d::Layer
        {
            
            friend class EditScene;
                   
        public:
            
            explicit GMXLayer(EditScene& imguiLayer, GMXFile& file);
            virtual ~GMXLayer();
            
            static GMXLayer* create(EditScene& imguiLayer, GMXFile& file);
            virtual bool init() override;
            void updateCocosLogic();
            
            GMXFile& getFile() { return _file; }
            void setTile(int x, int y, const Tileset& tile, bool isExecCommand = false);
            const Tileset& getTile(int x, int y) const { return _tiles[y][x]; }
            
            void updateChunk(const cocos2d::Vec2& pivot);
            bool isUpdateChunk(const cocos2d::Vec2& newPos, const cocos2d::Vec2& oldPos);
            
            void putTile(TileType type, int x, int y);
            
            void setLayerPosition(const cocos2d::Vec2& pos) { _layerPosition = pos; }
            cocos2d::Vec2 getTileRootWorldPosition() const { return _tileRootWorldPosition; }
            cocos2d::Vec2 getCameraPosition() const { return _camera->getPosition(); }
            cocos2d::Vec2 getMousePosInWorld() const { return _mousePosInWorld; }
            cocos2d::Vec2 getCenterViewParameter() const { return _centerViewParam; }
            void setCenterViewParameter(const cocos2d::Vec2& p);
            cocos2d::Size getLayerSize() const { return _layerSize; }
            cocos2d::Size getWorldSize() const { return _file.worldSize; }
            
            void initFile();
            void showLayer(bool& opened);
            
            bool& isShowPalette() { return _isShowPalette; }
            bool& isShowNavigator() { return _isShowNavigator; }
            bool& isShowHistory() { return _isShowHistory; }
            bool& isShowTriggerEdit() { return _isShowTriggerEdit; }
            bool& isShowForceSetting() { return _isShowForceSetting; }
            bool& isShowPropertyEdit() { return _isShowPropertyEdit; }
            
            bool isRedo() const;
            bool isUndo() const;
            void redo();
            void undo();
            
            PaletteLayer* getPaletteLayer() const { return _paletteLayer; }
            TriggerEditLayer* getTriggerEditLayer() const { return _triggerEditLayer; }
            
            void setCommand(CommandBase* newCommand);
            TileToolCommand* getTileToolCommand() const { return _tileToolCommand; }
            AddEntityToolCommand* getAddEntityToolCommand() const { return _addEntityToolCommand; }
            
            bool addEntity(EditorEntity* entity, int localZOrder = 0, bool isExecCommand = false);
            bool addEntityForce(EditorEntity* entity, int localZOrder = 0);
            bool eraseEntity(int id, bool isExecCommand = false);
            int getNumberOfHumanEntity(PlayerType player) const;
            EditorEntity* getHumanEntity(PlayerType player) const;
            EditorEntity* getEntityFromID(int id);
            
            int getNextValidID();
            static void enableTitleClicked() { TITLE_CLICKED = true; }
            static void disableTitleClicked() { TITLE_CLICKED = false; }
            static bool isTitleClicked() { return TITLE_CLICKED; }
            
            void removeSelectedEntities(bool isExecCommand = false);
            void clearSelectedEntites();
            
            void initCollisionData();
            void updateCollisionRegion();
            void setVisibleCollisionRegions(bool visible) { _collisionNode->setVisible(visible); }
            
            bool isFirstFile() const { return _isFirstFile; }
            void enableFirstFile(bool enable) { _isFirstFile = enable; }
            
            std::string getCurrFilePath() const { return _currFilePath; }
            void setCurrFilePath(const std::string& path) { _currFilePath = path; }
            
            void save(const std::string& path);
            
            bool addLocation(LocationNode* node);
            bool removeLocation(LocationNode* node);
            LocationNode* findLocation(const std::string& name);
            void setVisibleLocations(bool visible);
            void reorderLocations();
            LocationNode* getGrabbedLocation() const { return _grabbedLocation; }
            bool isOverlappedLocationName(const std::string& name) const;
            const std::vector<LocationNode*>& getLocations() const { return _locations; }
            
            void addTrigger(GameTrigger* trigger);
            PlayerInfo* getPlayerInfos() { return _playerInfos; }
            
        private:
            
            std::string getInDirection(int x, int y);
            std::string getOutDirection(const std::string& tail);
            std::pair<int, int> getNextTileIndex(const std::string& tailWithInputDir, int x, int y);
            
        private:
            
            static bool TITLE_CLICKED;
            
            EditScene& _imguiLayer;
            GMXFile& _file;
            
            cocos2d::DrawNode* _worldDebugNode;
            cocos2d::DrawNode* _localDebugNode;
            cocos2d::DrawNode* _hoveredTileRegion;
            cocos2d::ClippingRectangleNode* _clipNode;
            cocos2d::Size _layerSize;
            cocos2d::Vec2 _layerPosition;
            cocos2d::Vec2 _centerViewParam;
            cocos2d::Vec2 _tileRootWorldPosition;
            cocos2d::Vec2 _cameraDirection;
            cocos2d::Node* _camera;
            float _windowSpeed;
            cocos2d::Node* _tileRoot;
            cocos2d::Node* _rootNode;
            cocos2d::Sprite* _selectedItem;
            cocos2d::DrawNode* _selectedItemBoundingCircle;
            
            EditorCellSpacePartition* _cellSpacePartition;
            std::map<int, EditorEntity*> _entities;
            std::vector< std::vector<Tileset> > _tiles;
            std::vector< std::vector<TileImage*> > _tileImages;
            std::vector< EditorEntity* > _selectedEntities;
            
            bool _isPropertyPopupOpened = false;
            
            
            int _viewX;
            int _viewY;
            
            bool _isLeftMouseClickEventDone = false;
            
            cocos2d::Vec2 _mousePosInWorld;
            std::string _selectedEntitiesInfomation;
            bool _isPopupModal = false;
            
            PaletteLayer* _paletteLayer = nullptr;
            bool _isShowPalette = true;
            
            NavigatorLayer* _navigatorLayer = nullptr;
            bool _isShowNavigator = true;
            
            HistoryLayer* _historyLayer = nullptr;
            bool _isShowHistory = true;
            
            RenameLocationLayer* _renameLocationLayer = nullptr;
            bool _isShowRenameLocationLayer = false;
            
            TriggerEditLayer* _triggerEditLayer = nullptr;
            bool _isShowTriggerEdit = false;
            
            ForceSettingLayer* _forceSettingLayer = nullptr;
            bool _isShowForceSetting = false;
            
            PropertyEditLayer* _propertyEditLayer = nullptr;
            bool _isShowPropertyEdit = false;
            
            CommandBase* _currCommand = nullptr;
            TileToolCommand* _tileToolCommand = nullptr;
            AddEntityToolCommand* _addEntityToolCommand = nullptr;
            RemoveEntityToolCommand* _removeEntityToolCommand = nullptr;
            ModiftStartingPointCommand* _modifyStartingPointCommand = nullptr;
            
            cocos2d::Rect _selectRect;
            cocos2d::DrawNode* _selectionRectNode;
            
            std::vector<LocationNode*> _locations;
            LocationNode* _grabbedLocation = nullptr;
            
            std::map<std::string, std::vector<cocos2d::Vec2>> _tileCollisions;
            std::map<std::string, std::vector<cocos2d::Vec2>> _tileSimpleCollisions;
            std::vector< std::vector<cocos2d::Vec2> > _collisionRegions;
            std::vector< std::vector<cocos2d::Vec2> > _simpleCollisionRegions;
            cocos2d::DrawNode* _collisionNode;
            
            bool _isFirstFile = true;
            std::string _currFilePath;
            
            PlayerInfo _playerInfos[9];
            
        };
        
    }
}









