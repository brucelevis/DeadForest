//
//  TriggerEditor.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 22..
//
//

#ifndef TriggerEditor_hpp
#define TriggerEditor_hpp

#include "cocos2d.h"
#include "imgui.h"

class EditScene;

class TriggerEditor : public cocos2d::Node
{
    
public:
    
    explicit TriggerEditor(EditScene* layer);
    
    virtual ~TriggerEditor();
    
    static TriggerEditor* create(EditScene* layer);
    
    void showTriggerEditor(bool* opened);
    
private:
    
    EditScene* _imguiLayer;
    
};

#endif /* TriggerEditor_hpp */
