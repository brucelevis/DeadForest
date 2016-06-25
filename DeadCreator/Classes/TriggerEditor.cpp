//
//  TriggerEditor.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 22..
//
//

#include "TriggerEditor.hpp"

TriggerEditor::TriggerEditor()
{
}


TriggerEditor::~TriggerEditor()
{
}


TriggerEditor* TriggerEditor::create()
{
    auto ret = new (std::nothrow) TriggerEditor();
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_FREE(ret);
    return nullptr;
}


void TriggerEditor::showTriggerEditor(bool* opened)
{
    auto visibleSize = _director->getVisibleSize();
    Vec2 windowSize = Vec2(800, 600);
    ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y));
    ImGui::SetNextWindowPos(ImVec2((visibleSize.width - windowSize.x) / 2, (visibleSize.height - windowSize.y) / 2), ImGuiSetCond_FirstUseEver);
    
    ImGui::Begin("Trigger Editor", opened, ImGuiWindowFlags_NoCollapse);

    
    ImGui::Text("It is Trigger Editor");
    ImGui::Separator();
    
    static char text[1024*16] =
    "/*\n"
    " The Pentium F00F bug, shorthand for F0 0F C7 C8,\n"
    " the hexadecimal encoding of one offending instruction,\n"
    " more formally, the invalid operand with locked CMPXCHG8B\n"
    " instruction bug, is a design flaw in the majority of\n"
    " Intel Pentium, Pentium MMX, and Pentium OverDrive\n"
    " processors (all in the P5 microarchitecture).\n"
    "*/\n\n"
    "label:\n"
    "\tlock cmpxchg8b eax\n";
    
    ImGui::InputTextMultiline("##source", text, ((int)(sizeof(text)/sizeof(*text))),
                              ImVec2(-1.0f, ImGui::GetTextLineHeight() * 28),
                              ImGuiInputTextFlags_AllowTabInput);
    
    ImGui::End();
}









