#ifndef __CC_IMGUI_H__
#define __CC_IMGUI_H__

#include "cocos2d.h"
#include "imgui.h"

#include <boost/any.hpp>


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
	void setValue(boost::any value, std::string uid);
    boost::any getValue(std::string uid) const;
	void removeValue(std::string uid);
    
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
    std::map<std::string,  boost::any> _values;
    std::unordered_map<unsigned int, int> _usedTextureIdMap;
    
    //-------------------------------------------------------
    bool _isShowSetupStyle = false;
    void displaySetupStyle();
};

#endif // __IMGUILAYER_H__
