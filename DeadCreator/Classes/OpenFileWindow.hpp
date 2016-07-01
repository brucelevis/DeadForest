////
////  OpenFileWindow.hpp
////  DeadCreator
////
////  Created by mac on 2016. 6. 24..
////
////
//
//#ifndef OpenFileWindow_hpp
//#define OpenFileWindow_hpp
//
//#include "cocos2d.h"
//#include "imgui.h"
//#include "imgui_internal.h"
//
//class EditScene;
//
//class OpenFileWindow : public cocos2d::Node
//{
//    
//public:
//    
//    explicit OpenFileWindow(EditScene* layer);
//    
//    virtual ~OpenFileWindow();
//    
//    static OpenFileWindow* create(EditScene* layer);
//    
//    void showOpenFileWindow(bool* opened);
//    
//private:
//    
//    void closeWindow();
//    
//    void showDirectoryAndFile(const std::string& path);
//    
//private:
//    
//    EditScene* _imguiLayer;
//    
//    char _filePath[256];
//    
//    std::vector<std::string> _inDirectories;
//    
//    std::vector<std::string> _inGMXFiles;
//    
//    ImGuiButtonFlags _openButtonFlags = ImGuiButtonFlags_Disabled;
//    
//    float _openButtonTextAlpha = 0.5f;
//    
//    std::string _lastFileRoot;
//    
//};
//
//#endif /* OpenFileWindow_hpp */
