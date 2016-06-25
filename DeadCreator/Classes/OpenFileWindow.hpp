//
//  OpenFileWindow.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 24..
//
//

#ifndef OpenFileWindow_hpp
#define OpenFileWindow_hpp

#include "cocos2d.h"
#include "CCImGui.h"

class OpenFileWindow : public cocos2d::Node
{
    
public:
    
    OpenFileWindow();
    
    virtual ~OpenFileWindow();
    
    static OpenFileWindow* create();
    
    void showOpenFileWindow(bool* opened);
    
private:
    
    static int TextEditCallBackStub(ImGuiTextEditCallbackData* data);
    
    int TextEditCallBack(ImGuiTextEditCallbackData* data);
    
    int strnicmp(const char* str1, const char* str2, int n);
    
private:
    
    
    char _filePath[256];
    
    std::vector<std::string> _inDirectories;
    
    bool _isInputCompleted = false;
    
};

#endif /* OpenFileWindow_hpp */
