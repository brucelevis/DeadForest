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

class GMXFile;
class GMXLayer2;
class NewFileWindow2;

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
    
private:
    
    GMXLayer2* _layer;
    
    NewFileWindow2* _newFileWindow;

    bool _showNewMap = false;
    
    bool _isFileEnable = true;
    
    bool _isEditEnable = false;
    
    bool _isPlayerEnable = false;
    
    bool _isWindowEnable = false;
    
};
