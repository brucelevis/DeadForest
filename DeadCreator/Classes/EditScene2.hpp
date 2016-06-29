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

class GMXLayer2;
class NewFileWindow2;

class EditScene2 : public ImGuiLayer
{
    
public:
    
    CREATE_FUNC(EditScene2);
    
    static cocos2d::Scene* createScene();
    
    virtual bool init() override;
    
private:
    
    GMXLayer2* _layer;
    
    NewFileWindow2* _newFileWindow;

    bool _showNewMap = false;
    
};
