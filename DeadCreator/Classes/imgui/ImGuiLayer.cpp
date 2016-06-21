#include "ImGuiLayer.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "CCImGui.h"

USING_NS_CC;

bool ImGuiLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	//----------------------------------------
	// init imgui cc
	CCIMGUI->setWindow(((GLViewImpl*)Director::getInstance()->getOpenGLView())->getWindow());
    setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
    
	//----------------------------------------
	// events
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Touch* touch, Event*) -> bool {
        
        bool inImGuiWidgets = ImGui::IsPosHoveringAnyWindow(ImVec2(touch->getLocationInView().x,
                                                                   touch->getLocationInView().y));
//        log("touch in ImGui widgets %s", inImGuiWidgets ? "yes" : "no");
        return inImGuiWidgets;
        
    };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}


void ImGuiLayer::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags)
{
    Layer::visit(renderer, parentTransform, parentFlags);
    _command.init(_globalZOrder);
    _command.func = CC_CALLBACK_0(ImGuiLayer::onDraw, this);
    Director::getInstance()->getRenderer()->addCommand(&_command);
}


void ImGuiLayer::onDraw()
{
    glUseProgram(0);
    if (CCIMGUI->getWindow())
    {
		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = Director::getInstance()->getDeltaTime();

        ImGui_ImplGlfw_NewFrame();
		CCIMGUI->updateImGUI();
        // Rendering
        ImGui::Render();
    }
	glUseProgram(1);
}
