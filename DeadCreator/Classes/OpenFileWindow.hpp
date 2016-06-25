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
#include "imgui.h"

class EditScene;

class OpenFileWindow : public cocos2d::Node
{
    
public:
    
    explicit OpenFileWindow(EditScene* layer);
    
    virtual ~OpenFileWindow();
    
    static OpenFileWindow* create(EditScene* layer);
    
    void showOpenFileWindow(bool* opened);
    
private:
    
    static int TextEditCallBackStub(ImGuiTextEditCallbackData* data);
    
    int TextEditCallBack(ImGuiTextEditCallbackData* data);
    
    int strnicmp(const char* str1, const char* str2, int n);
    
    void closeWindow();
    
    void showDirectoryAndFile(const std::string& path);
    
private:
    
    EditScene* _imguiLayer;
    
    char _filePath[256];
    
    std::vector<std::string> _inDirectories;
    
    std::vector<std::string> _inGMXFiles;
    
};

#endif /* OpenFileWindow_hpp */
