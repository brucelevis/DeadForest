//
//  HistoryLayer.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 3..
//
//

#pragma once

#include "cocos2d.h"
#include "ImGuiLayer.h"
#include "CommandQueue.hpp"

namespace realtrick
{
    
    class GMXLayer2;
    
    class HistoryLayer : public cocos2d::Node
    {
        
    public:
        
        HistoryLayer(GMXLayer2& gmxLayer);
        
        virtual ~HistoryLayer();
        
        static HistoryLayer* create(GMXLayer2& gmxLayer);
        
        virtual bool init() override;
        
        void showLayer(bool* opened);
        
        void pushCommand(CommandBase* command) { _commandQueue.pushCommand(command); _isSetScrollBottom = true; }
        
        void redo() { if ( isRedo() ) _commandQueue.redo(); }
        
        void undo() { if ( isUndo() ) _commandQueue.undo(); }
        
        int getIndex() const { return _commandQueue.getIndex(); }
        
        bool isRedo() const { return _commandQueue.isRedo(); }
        
        bool isUndo() const { return _commandQueue.isUndo(); }
        
        int size() const { return _commandQueue.size(); }
        
    private:
        
        GMXLayer2& _gmxLayer;
        
        cocos2d::Size _layerSize;
        cocos2d::Vec2 _layerPosition;
        cocos2d::Rect _boundingBoxPadding;
        
        CommandQueue _commandQueue;
        bool _isSetScrollBottom = false;
        
    };
    
}








