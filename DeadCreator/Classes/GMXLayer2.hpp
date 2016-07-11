//
//  GMXLayer2.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 28..
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "GMXFile.hpp"
#include "EditorCellSpacePartition.hpp"
#include "TileImage.hpp"
#include "TileBase.hpp"
#include "CommandQueue.hpp"
#include "SizeProtocol.h"
#include "HistoryLayer.hpp"

#define DUMMY_TILE_SIZE 4

namespace realtrick
{
    
    class PaletteLayer;
    class NavigatorLayer;
    class EditScene2;
    class CommandBase;
    class AddEntityToolCommand;
    class RemoveEntityToolCommand;
    class TileToolCommand;
    class EditorEntityBase;
    
    class GMXLayer2 : public cocos2d::Layer
    {
        
    public:
        
        explicit GMXLayer2(EditScene2& _imguiLayer, GMXFile& file);
        
        virtual ~GMXLayer2();
        
        static GMXLayer2* create(EditScene2& imguiLayer, GMXFile& file);
        
        virtual bool init() override;
        
        virtual void update(float dt) override;
        
        void updateCocosLogic();
        
        const GMXFile& getFile() const { return _file; }
        
        void setTile(int x, int y, const TileBase& tile, bool isExecCommand = false);
        
        void showWindow();
        
        void updateChunk(const cocos2d::Vec2& pivot);
        
        bool isUpdateChunk(const cocos2d::Vec2& newPos, const cocos2d::Vec2& oldPos);
        
        void putTile(EditorTileType type, int x, int y);
        
        void setLayerPosition(const cocos2d::Vec2& pos) { _layerPosition = pos; }
        
        cocos2d::Vec2 getTileRootWorldPosition() const { return _tileRootWorldPosition; }
        
        cocos2d::Vec2 getCameraPosition() const { return _camera->getPosition(); }
        
        cocos2d::Vec2 getMousePosInWorld() const { return _mousePosInWorld; }
        
        cocos2d::Vec2 getCenterViewParameter() const { return _centerViewParam; }
        
        void setCenterViewParameter(const cocos2d::Vec2& p);
        
        cocos2d::Size getLayerSize() const { return _layerSize; }
        
        cocos2d::Size getWorldSize() const { return _file.worldSize; }
        
        const TileBase& getTile(int x, int y) const { return _tiles[y][x]; }
        
        void initFile();
        
        bool& isShowPalette() { return _isShowPalette; }
        bool& isShowNavigator() { return _isShowNavigator; }
        bool& isShowHistory() { return _isShowHistory; }
        
        bool isRedo() const { return _historyLayer->isRedo(); }
        bool isUndo() const { return _historyLayer->isUndo(); }
        void redo() { _historyLayer->redo(); updateCollisionRegion(); }
        void undo() { _historyLayer->undo(); updateCollisionRegion(); }
        
        PaletteLayer* getPaletteLayer() const { return _paletteLayer; }
        
        void setCommand(CommandBase* newCommand);
        TileToolCommand* getTileToolCommand() const { return _tileToolCommand; }
        AddEntityToolCommand* getAddEntityToolCommand() const { return _addEntityToolCommand; }
        
        bool addEntity(EditorEntityBase* entity, int localZOrder = 0, bool isExecCommand = false);
        
        bool eraseEntity(int id, bool isExecCommand = false);
        
        static int getNextValidID()
        {
            static int validID = 0;
            return validID++;
        }
        
        static void enableTitleClicked() { TITLE_CLICKED = true; }
        static void disableTitleClicked() { TITLE_CLICKED = false; }
        static bool isTitleClicked() { return TITLE_CLICKED; }
        
        void enableEntityBoundingBoxNode(bool enable);
        
        void removeSelectedEntities(bool isExecCommand = false);
        void clearSelectedEntites();
        
        void initCollisionData();
        void updateCollisionRegion();
        
        bool isFirstFile() const { return _isFirstFile; }
        void enableFirstFile(bool enable) { _isFirstFile = enable; }

        std::string getCurrFilePath() const { return _currFilePath; }
        void setCurrFilePath(const std::string& path) { _currFilePath = path; }
        
        void save(const std::string& path);
        
    private:
        
        std::string getInDirection(int x, int y);
        std::string getOutDirection(const std::string& tail);
        std::pair<int, int> getNextTileIndex(const std::string& tailWithInputDir, int x, int y);
        
    private:
        
        static bool TITLE_CLICKED;
        
        EditScene2& _imguiLayer;
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
        
        EditorCellSpacePartition* _cellSpacePartition;
        std::map<int, EditorEntityBase*> _entities;
        std::vector<std::vector<TileBase>> _tiles;
        std::vector< std::vector<TileImage*> > _tileImages;
        std::vector< std::vector<cocos2d::ui::Text*> > _tileIndices;
        std::vector< EditorEntityBase* > _selectedEntities;
        int _viewX;
        int _viewY;
        
        bool _isShowWindow = true;
        bool _isLeftMouseClickEventDone = false;
        
        cocos2d::Vec2 _mousePosInWorld;
        cocos2d::Vec2 _mousePosInCanvas;
        
        PaletteLayer* _paletteLayer = nullptr;
        bool _isShowPalette = true;
        
        NavigatorLayer* _navigatorLayer = nullptr;
        bool _isShowNavigator = true;
        
        HistoryLayer* _historyLayer = nullptr;
        bool _isShowHistory = true;
        
        CommandBase* _currCommand = nullptr;
        TileToolCommand* _tileToolCommand = nullptr;
        AddEntityToolCommand* _addEntityToolCommand = nullptr;
        RemoveEntityToolCommand* _removeEntityToolCommand = nullptr;
        
        cocos2d::Rect _selectRect;
        cocos2d::DrawNode* _selectionRectNode;
        
        std::map<std::string, std::vector<cocos2d::Vec2>> _tileCollisions;
        std::vector< std::vector<cocos2d::Vec2> > _collisionRegions;
        cocos2d::DrawNode* _collisionNode;
        
        bool _isFirstFile = true;
        std::string _currFilePath;
        
    };
    
}










