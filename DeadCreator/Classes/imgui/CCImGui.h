#ifndef __CC_IMGUI_H__
#define __CC_IMGUI_H__

#include "cocos2d.h"
#include "imgui.h"

USING_NS_CC;


#define CCIMGUI CCImGui::getInstance()

class CCImGui
{
    
public:
    
	static CCImGui* getInstance();
    
	//-------------------------------------------------------
	GLFWwindow* getWindow() const { return _window; };
	void setWindow(GLFWwindow* window) { _window = window; };
	ImVec4 getClearColor() const { return _clearColor; };
	void setClearColor(ImColor color) { _clearColor = color; };
    
	//-------------------------------------------------------
	void init();
	void updateImGUI();
	void addImGUI(std::function<void()> imGUICall, std::string name) { _callPiplines[name] = imGUICall; };
	bool removeImGUI(std::string name);
    
    //-------------------------------------------------------
    void image(const std::string& fn, int w = -1, int h = -1);
    bool imageButton(const std::string& fn, int w = -1, int h = -1);
    
private:
    
    //-------------------------------------------------------
    CCImGui() = default;
    virtual ~CCImGui() = default;
    CCImGui(const CCImGui& rhs) = delete;
    CCImGui(CCImGui&& rhs) = delete;
    CCImGui& operator=(const CCImGui& rhs) = delete;
    CCImGui& operator=(CCImGui&& rhs) = delete;
    
    //-------------------------------------------------------
    GLFWwindow* _window = nullptr;
    ImVec4 _clearColor = ImColor(114, 144, 154);
    
    //-------------------------------------------------------
    std::map<std::string, std::function<void()>> _callPiplines;
    std::unordered_map<unsigned int, int> _usedTextureIdMap;
    
};

#endif // __IMGUILAYER_H__
