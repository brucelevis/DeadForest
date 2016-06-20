#include "CCImGui.h"

USING_NS_CC;
static CCImGui* _instance = NULL;
//================================================
// style editor variables
static int hue = 140;
static float col_main_sat = 180.f / 255.f;
static float col_main_val = 161.f / 255.f;
static float col_area_sat = 124.f / 255.f;
static float col_area_val = 100.f / 255.f;
static float col_back_sat = 59.f / 255.f;
static float col_back_val = 40.f / 255.f;

CCImGui* CCImGui::getInstance()
{
	if(_instance == NULL)
	{
		_instance = new (std::nothrow) CCImGui();
		_instance->init();
	}
	return _instance;
}

void CCImGui::init()
{
	_callPiplines["styleEditor"] = std::bind(&CCImGui::displaySetupStyle, this);
	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowFillAlphaDefault = 0.98f;
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
    
    style.FramePadding = ImVec2(4.0f, 4.0f);
    
    ImGui::GetIO().FontGlobalScale = 1.4f;

}

void CCImGui::updateImGUI()
{
    for (auto &d : _callPiplines)
	{
		d.second();
	}
}

bool CCImGui::removeImGUI(std::string name)
{
	auto iter = _callPiplines.find(name);
    if (iter != _callPiplines.end())
    {
		_callPiplines.erase(iter);
        return true;
    }
    return false;
}

void CCImGui::setValue(bool value, std::string uid)
{
	CCImGuiValue* v = new (std::nothrow)CCImGuiValue();
	v->value = (void*)value;
	_values[uid] = v;
}

void CCImGui::setValue(long long value, std::string uid)
{
	CCImGuiValue* v = new (std::nothrow)CCImGuiValue();
	v->value = (void*)value;
	_values[uid] = v;
}

CCImGuiValue* CCImGui::getValue(std::string uid) const
{
	return _values.at(uid);
}

bool CCImGui::removeValue(std::string uid)
{
    auto del = getValue(uid);
    if ( del )
    {
        CC_SAFE_DELETE(del);
        _values.erase(uid);
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

void CCImGui::image(const std::string& fn, int w, int h)
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
bool CCImGui::imageButton(const std::string& fn, int w, int h)
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

void CCImGui::displaySetupStyle()
{
	if (_isShowSetupStyle)
    {
		ImGui::Begin("Hue Style", &_isShowSetupStyle);
		ImGui::SliderInt("master hue", &hue, 0, 255);

		float dummy;
		ImVec4 rgb;
		ImGui::ColorEditMode(ImGuiColorEditMode_HSV);

		ImGui::ColorConvertHSVtoRGB(hue / 255.f, col_main_sat, col_main_val, rgb.x, rgb.y, rgb.z);
		ImGui::ColorEdit3("main", &rgb.x);
		ImGui::ColorConvertRGBtoHSV(rgb.x, rgb.y, rgb.z, dummy, col_main_sat, col_main_val);

		ImGui::ColorConvertHSVtoRGB(hue / 255.f, col_area_sat, col_area_val, rgb.x, rgb.y, rgb.z);
		ImGui::ColorEdit3("area", &rgb.x);
		ImGui::ColorConvertRGBtoHSV(rgb.x, rgb.y, rgb.z, dummy, col_area_sat, col_area_val);

		ImGui::ColorConvertHSVtoRGB(hue / 255.f, col_back_sat, col_back_val, rgb.x, rgb.y, rgb.z);
		ImGui::ColorEdit3("back", &rgb.x);
		ImGui::ColorConvertRGBtoHSV(rgb.x, rgb.y, rgb.z, dummy, col_back_sat, col_back_val);

		ImGui::End();

		ImGuiStyle& style = ImGui::GetStyle();

		ImVec4 col_text = ImColor::HSV(hue / 255.f, 20.f / 255.f, 235.f / 255.f);
		ImVec4 col_main = ImColor::HSV(hue / 255.f, col_main_sat, col_main_val);
		ImVec4 col_back = ImColor::HSV(hue / 255.f, col_back_sat, col_back_val);
		ImVec4 col_area = ImColor::HSV(hue / 255.f, col_area_sat, col_area_val);
		style.WindowFillAlphaDefault = 1.0f;
		style.Colors[ImGuiCol_Text] = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(col_text.x, col_text.y, col_text.z, 0.58f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(col_back.x, col_back.y, col_back.z, 1.00f);
		style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(col_area.x, col_area.y, col_area.z, 0.00f);
		style.Colors[ImGuiCol_Border] = ImVec4(col_text.x, col_text.y, col_text.z, 0.30f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.68f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.45f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(col_main.x, col_main.y, col_main.z, 0.35f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(col_area.x, col_area.y, col_area.z, 0.57f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(col_main.x, col_main.y, col_main.z, 0.31f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
		style.Colors[ImGuiCol_ComboBg] = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(col_main.x, col_main.y, col_main.z, 0.80f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(col_main.x, col_main.y, col_main.z, 0.24f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(col_main.x, col_main.y, col_main.z, 0.44f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.86f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(col_main.x, col_main.y, col_main.z, 0.76f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.86f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
		style.Colors[ImGuiCol_Column] = ImVec4(col_text.x, col_text.y, col_text.z, 0.32f);
		style.Colors[ImGuiCol_ColumnHovered] = ImVec4(col_text.x, col_text.y, col_text.z, 0.78f);
		style.Colors[ImGuiCol_ColumnActive] = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(col_main.x, col_main.y, col_main.z, 0.20f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
		style.Colors[ImGuiCol_CloseButton] = ImVec4(col_text.x, col_text.y, col_text.z, 0.16f);
		style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(col_text.x, col_text.y, col_text.z, 0.39f);
		style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.43f);
		style.Colors[ImGuiCol_TooltipBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.92f);
		style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
	}
}