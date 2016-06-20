#ifndef __CC_IMGUI_H__
#define __CC_IMGUI_H__

#include "cocos2d.h"
#include "imgui.h"

USING_NS_CC;

class CCImGuiValue
{
public:
	void* value = nullptr;
	bool getBool() { return (bool)value; };
    long long getInt() { return (long long)value; };
};

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
	void setValue(bool value, std::string uid);
	void setValue(long long value, std::string uid);
	CCImGuiValue* getValue(std::string uid) const;
	bool removeValue(std::string uid);
    
    //-------------------------------------------------------
    void image(const std::string& fn, int w = -1, int h = -1);
    bool imageButton(const std::string& fn, int w = -1, int h = -1);
    
	//-------------------------------------------------------
	void setShowStyleEditor(bool show) { _isShowSetupStyle = show; };
    
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
    std::map<std::string, CCImGuiValue*> _values;
    std::unordered_map<unsigned int, int> _usedTextureIdMap;
    
    //-------------------------------------------------------
    bool _isShowSetupStyle = false;
    void displaySetupStyle();
};

#endif // __IMGUILAYER_H__
