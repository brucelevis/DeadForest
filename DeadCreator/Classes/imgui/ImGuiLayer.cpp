#include "ImGuiLayer.h"

USING_NS_CC;

bool ImGuiLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
  
    setWindow(((GLViewImpl*)Director::getInstance()->getOpenGLView())->getWindow());
    setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
    
    ImGuiStyle& style = ImGui::GetStyle();
    
    style.Colors[ImGuiCol_Text] = ImVec4(0.0000000, 0.0000000, 0.0000000, 1.0000000);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.6000000, 0.6000000, 0.6000000, 1.0000000);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.9400000, 0.9400000, 0.9400000, 1.0000000);
    style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.0000000, 0.0000000, 0.0000000, 0.0000000);
    style.Colors[ImGuiCol_Border] = ImVec4(0.0000000, 0.0000000, 0.0000000, 0.3900000);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.0000000, 1.0000000, 1.0000000, 0.1000000);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(1.0000000, 1.0000000, 1.0000000, 1.0000000);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2600000, 0.5900000, 0.9800000, 0.4000000);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2600000, 0.5900000, 0.9800000, 0.6700000);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.9599999, 0.9599999, 0.9599999, 1.0000000);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.0000000, 1.0000000, 1.0000000, 0.5100000);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.8200000, 0.8200000, 0.8200000, 1.0000000);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.8600000, 0.8600000, 0.8600000, 1.0000000);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.9800000, 0.9800000, 0.9800000, 0.5300000);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.6900000, 0.6900000, 0.6900000, 0.8000000);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4900000, 0.4900000, 0.4900000, 0.8000000);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.4900000, 0.4900000, 0.4900000, 1.0000000);
    style.Colors[ImGuiCol_ComboBg] = ImVec4(0.8600000, 0.8600000, 0.8600000, 0.9900000);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.2600000, 0.5900000, 0.9800000, 1.0000000);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.2600000, 0.5900000, 0.9800000, 0.7800000);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.2600000, 0.5900000, 0.9800000, 1.0000000);
    style.Colors[ImGuiCol_Button] = ImVec4(0.2600000, 0.5900000, 0.9800000, 0.4000000);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.2600000, 0.5900000, 0.9800000, 1.0000000);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.0600000, 0.5300000, 0.9800000, 1.0000000);
    style.Colors[ImGuiCol_Header] = ImVec4(0.2600000, 0.5900000, 0.9800000, 0.3100000);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.2600000, 0.5900000, 0.9800000, 0.8000000);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.2600000, 0.5900000, 0.9800000, 1.0000000);
    style.Colors[ImGuiCol_Column] = ImVec4(0.3900000, 0.3900000, 0.3900000, 1.0000000);
    style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.2600000, 0.5900000, 0.9800000, 0.7800000);
    style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.2600000, 0.5900000, 0.9800000, 1.0000000);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.8200000, 0.8200000, 0.8200000, 1.0000000);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2600000, 0.5900000, 0.9800000, 0.6700000);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.2600000, 0.5900000, 0.9800000, 0.9500000);
    style.Colors[ImGuiCol_CloseButton] = ImVec4(0.5900000, 0.5900000, 0.5900000, 0.5000000);
    style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.9800000, 0.3900000, 0.3600000, 1.0000000);
    style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.9800000, 0.3900000, 0.3600000, 1.0000000);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.3900000, 0.3900000, 0.3900000, 1.0000000);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0000000, 0.4300000, 0.3500000, 1.0000000);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.9000000, 0.7000000, 0.0000000, 1.0000000);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0000000, 0.6000000, 0.0000000, 1.0000000);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2600000, 0.5900000, 0.9800000, 0.3500000);
    style.Colors[ImGuiCol_TooltipBg] = ImVec4(1.0000000, 1.0000000, 1.0000000, 0.9400000);
    style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.2000000, 0.2000000, 0.2000000, 0.3500000);
    
    style.WindowFillAlphaDefault = 0.98f;
    style.WindowPadding.x = 4.0f;
    style.WindowPadding.y = 4.0f;
    style.FramePadding = ImVec2(4.0f, 4.0f);
    style.FrameRounding = 4.0f;
    style.ItemSpacing.x = 8.0f;
    
    ImGui::GetIO().FontGlobalScale = 1.4f;
    
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
    if (getWindow())
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DeltaTime = Director::getInstance()->getDeltaTime();
        
        ImGui_ImplGlfw_NewFrame();
        
        for (auto &d : _callPiplines)
        {
            d.second();
        }
        
        ImGui::Render();
    }
    glUseProgram(1);
}

bool ImGuiLayer::removeImGUI(std::string name)
{
    auto iter = _callPiplines.find(name);
    if (iter != _callPiplines.end())
    {
        _callPiplines.erase(iter);
        return true;
    }
    return false;
}


void ImGuiLayer::image(const std::string& fn, float w, float h)
{
    auto texName = cocos2d::Director::getInstance()->getTextureCache()->addImage(fn)->getName();
    ImGui::Image(reinterpret_cast<ImTextureID>(texName), ImVec2(w, h));
}

bool ImGuiLayer::imageButton(const std::string& fn,float w, float h)
{
    auto texName = cocos2d::Director::getInstance()->getTextureCache()->addImage(fn)->getName();
    return ImGui::ImageButton(reinterpret_cast<ImTextureID>(texName), ImVec2(w, h));
}









