#pragma once

#include "cocos2d.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"

class ImGuiLayer : public cocos2d::Layer
{

public:
    
    virtual bool init() override;

    virtual void visit(cocos2d::Renderer *renderer,
                       const cocos2d::Mat4& parentTransform,
                       uint32_t parentFlags) override;

    void onDraw();

    CREATE_FUNC(ImGuiLayer);
    
    GLFWwindow* getWindow() const { return _window; };
    
    void setWindow(GLFWwindow* window) { _window = window; };
    
    ImVec4 getClearColor() const { return _clearColor; };
    
    void setClearColor(ImColor color) { _clearColor = color; };
    
    void addImGUI(std::function<void()> imGUICall, std::string name) { _callPiplines[name] = imGUICall; };
    
    bool removeImGUI(std::string name);
    
    static void image(const std::string& fn, float w, float h);
    
    static bool imageButton(const std::string& fn,
                            float w,
                            float h,
                            const ImVec2& uv0 = ImVec2(0,0),
                            const ImVec2& uv1 = ImVec2(1,1),
                            int frame_padding = -1,
                            const ImVec4& bg_col = ImVec4(0,0,0,0),
                            const ImVec4& tint_col = ImVec4(1,1,1,1));

protected:
    
    cocos2d::CustomCommand _command;
    GLFWwindow* _window = nullptr;
    ImVec4 _clearColor = ImColor(114, 144, 154);
    std::map<std::string, std::function<void()>> _callPiplines;
    
};









