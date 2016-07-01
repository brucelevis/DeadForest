////
////  SaveAsFileWindow.hpp
////  DeadCreator
////
////  Created by mac on 2016. 6. 26..
////
////
//
//#ifndef SaveAsFileWindow_hpp
//#define SaveAsFileWindow_hpp
//
//#include "cocos2d.h"
//#include "imgui.h"
//#include "imgui_internal.h"
//
//class EditScene;
//
//class SaveAsFileWindow : public cocos2d::Node
//{
//    
//public:
//    
//    explicit SaveAsFileWindow(EditScene* layer);
//    
//    virtual ~SaveAsFileWindow();
//
//    static SaveAsFileWindow* create(EditScene* layer);
//    
//    virtual bool init() override;
//    
//    void showSaveAsFileWindow(bool* opened);
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
//    ImGuiButtonFlags _saveButtonFlags = ImGuiButtonFlags_Disabled;
//    
//    float _saveButtonTextAlpha = 0.5f;
//    
//    std::string _lastFileRoot;
//};
//
//#endif /* SaveAsFileWindow.hpp */
//
//
//
//
//
//
//
//
//
