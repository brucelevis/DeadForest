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

class ImGuiLayer;

class TriggerEditor : public cocos2d::Node
{
    
public:
    
    explicit TriggerEditor(ImGuiLayer* layer);
    
    virtual ~TriggerEditor();
    
    static TriggerEditor* create(ImGuiLayer* layer);
    
    void showTriggerEditor(bool* opened);
    
private:
    
    ImGuiLayer* _imguiLayer;
    
};

#endif /* TriggerEditor_hpp */
