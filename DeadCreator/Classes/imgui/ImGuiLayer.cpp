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
    style.WindowPadding.x = 8.0f;
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
        
        _usedTextureIdMap.clear();
        
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


#include <tuple>
static std::tuple<Texture2D*, ImVec2, ImVec2, ImVec2> getTextureInfo(const std::string& fn, int w = -1, int h = -1) {
    std::string name = fn;
    cocos2d::Texture2D *texture = NULL;
    ImVec2 uv0(0, 0);
    ImVec2 uv1(1, 1);
    ImVec2 size(0, 0);
    
    // sprite frame
    if (fn.at(0) == '#') {
        name = name.substr(1, name.size());
        SpriteFrame *sf = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
        if (sf) {
            float atlasWidth = (float)sf->getTexture()->getPixelsWide();
            float atlasHeight = (float)sf->getTexture()->getPixelsHigh();
            
            const Rect& rect = sf->getRect();
            texture = sf->getTexture();
            if (sf->isRotated()) {
                // FIXME:
                uv0.x = rect.origin.x / atlasWidth;
                uv0.y = rect.origin.y / atlasHeight;
                uv1.x = (rect.origin.x + rect.size.width) / atlasWidth;
                uv1.y = (rect.origin.y + rect.size.height) / atlasHeight;
            } else {
                uv0.x = rect.origin.x / atlasWidth;
                uv0.y = rect.origin.y / atlasHeight;
                uv1.x = (rect.origin.x + rect.size.width) / atlasWidth;
                uv1.y = (rect.origin.y + rect.size.height) / atlasHeight;
            }
            
            size.x = sf->getRect().size.width;
            size.y = sf->getRect().size.height;
        }
    } else {
        texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(fn);
        size.x = texture->getPixelsWide();
        size.y = texture->getPixelsHigh();
    }
    
    if (w > 0 && h > 0) {
        size.x = w;
        size.y = h;
    }
    
    return std::make_tuple(texture, size, uv0, uv1);
}

void ImGuiLayer::image(const std::string& fn, int w, int h)
{
    cocos2d::Texture2D *texture = NULL;
    ImVec2 uv0(0, 0);
    ImVec2 uv1(1, 1);
    ImVec2 size(0, 0);
    
    std::tie(texture, size, uv0, uv1) = getTextureInfo(fn, w, h);
    if (texture) {
        bool needToPopID = false;
        GLuint texId = texture->getName();
        if (_usedTextureIdMap.find(texId) == _usedTextureIdMap.end()) {
            _usedTextureIdMap[texId] = 0;
        } else {
            _usedTextureIdMap[texId]++;
            ImGui::PushID(_usedTextureIdMap[texId]);
            needToPopID = true;
        }
        
        ImGui::Image(reinterpret_cast<ImTextureID>(texId), size, uv0, uv1);
        
        if (needToPopID) {
            ImGui::PopID();
        }
    }
}
bool ImGuiLayer::imageButton(const std::string& fn, int w, int h)
{
    cocos2d::Texture2D *texture = NULL;
    ImVec2 uv0(0, 0);
    ImVec2 uv1(1, 1);
    ImVec2 size(0, 0);
    
    bool ret = false;
    std::tie(texture, size, uv0, uv1) = getTextureInfo(fn, w, h);
    if (texture) {
        bool needToPopID = false;
        GLuint texId = texture->getName();
        if (_usedTextureIdMap.find(texId) == _usedTextureIdMap.end()) {
            _usedTextureIdMap[texId] = 0;
        } else {
            _usedTextureIdMap[texId]++;
            ImGui::PushID(_usedTextureIdMap[texId]);
            needToPopID = true;
        }
        
        ret = ImGui::ImageButton(reinterpret_cast<ImTextureID>(texture->getName()), size, uv0, uv1);
        
        if (needToPopID) {
            ImGui::PopID();
        }
    }
    return ret;
}
