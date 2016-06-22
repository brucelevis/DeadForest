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
#include "CCImGui.h"

class TriggerEditor : public cocos2d::Node
{
    
public:
    
    TriggerEditor();
    
    virtual ~TriggerEditor();
    
    static TriggerEditor* create();
    
    void showTriggerEditor(bool* opened);
    
private:
    
    
    
};

#endif /* TriggerEditor_hpp */
