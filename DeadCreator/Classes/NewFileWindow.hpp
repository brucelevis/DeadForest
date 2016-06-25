//
//  NewFileWindow.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 24..
//
//

#ifndef NewFileWindow_hpp
#define NewFileWindow_hpp

#include "cocos2d.h"
#include "imgui.h"

class EditScene;

class NewFileWindow : public cocos2d::Node
{
    
public:
    
    explicit NewFileWindow(EditScene* layer);
    
    virtual ~NewFileWindow();
    
    static NewFileWindow* create(EditScene* layer);
    
    void showNewFileWindow(bool* opened);
    
private:
    
    void closeWindow();
    
private:
    
    EditScene* _imguilayer;
    
    int _tileSizeXItem = 0;
    int _tileSizeYItem = 0;
    int _numOfTileX = 0;
    int _numOfTileY = 0;
    int _currentTile = 0;

    
};

#endif /* NewFileWindow_hpp */
