//
//  OpenFileWindow.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 24..
//
//

#include <functional>

#include "OpenFileWindow.hpp"
#include "FileSystem.hpp"
using namespace cocos2d;

OpenFileWindow::OpenFileWindow()
{
}


OpenFileWindow::~OpenFileWindow()
{
}


OpenFileWindow* OpenFileWindow::create()
{
    auto ret = new (std::nothrow) OpenFileWindow();
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_FREE(ret);
    return nullptr;
}

int OpenFileWindow::strnicmp(const char* str1, const char* str2, int n)
{
    int d = 0;
    while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1)
    {
        str1++;
        str2++;
        n--;
    }
    return d;
}


void OpenFileWindow::showOpenFileWindow(bool* opened)
{
    auto visibleSize = _director->getVisibleSize();
    
    Vec2 windowSize = Vec2(700, 600);
    ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y));
    ImGui::SetNextWindowPos(ImVec2((visibleSize.width - windowSize.x) / 2, (visibleSize.height - windowSize.y) / 2), ImGuiSetCond_FirstUseEver);
    if (!ImGui::Begin("Open Map", opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
    {
        ImGui::End();
        return;
    }
    
    if (ImGui::InputText("search directory", _filePath, 256,
                     ImGuiInputTextFlags_CallbackCompletion |
                     ImGuiInputTextFlags_EnterReturnsTrue,
                     &TextEditCallBackStub, (void*)this))
    {
        log("enter!");
    }
    
    ImGui::Text("Press 'TAB' if input is over.");
    
    ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
    ImGui::BeginChild("##TileSize", ImVec2(0, 480), true);
    
    if ( _isInputCompleted )
    {
        if ( _inDirectories.size() == 0)
        {
            ImGui::Text("empty");
        }
        else
        {
            for (auto &dir : _inDirectories )
            {
                ImGui::Selectable(dir.c_str());
            }
        }
    }
    
    ImGui::EndChild();
    ImGui::PopStyleVar();
    
   
    ImGui::End();
}


int OpenFileWindow::TextEditCallBackStub(ImGuiTextEditCallbackData* data)
{
    OpenFileWindow* window = (OpenFileWindow*)data->UserData;
    return window->TextEditCallBack(data);
}


int OpenFileWindow::TextEditCallBack(ImGuiTextEditCallbackData* data)
{
    _inDirectories.clear();
    
    strncpy(_filePath, data->Buf, 256);
    
    if ( FileSystem::isExist(_filePath) )
    {
        _inDirectories = FileSystem::getDirectoryInPath(_filePath);
    }
    
    switch (data->EventFlag)
    {
        case ImGuiInputTextFlags_CallbackCompletion:
        {
            // Example of TEXT COMPLETION
            
            // Locate beginning of current word
            const char* word_end = data->Buf + data->CursorPos;
            const char* word_start = word_end;
            while (word_start > data->Buf)
            {
                const char c = word_start[-1];
                if (c == ' ' || c == '\t' || c == ',' || c == ';')
                    break;
                word_start--;
            }
            
            // Build a list of candidates
            std::vector<std::string> candidates;
            for (int i = 0; i < _inDirectories.size(); ++i)
                if (strnicmp(_inDirectories[i].c_str(), word_start, (int)(word_end-word_start)) == 0)
                    candidates.push_back(_inDirectories[i]);
            
            if ( candidates.size() == 0)
            {
                
            }
            else if (candidates.size() == 1)
            {
                // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
                data->DeleteChars((int)(word_start-data->Buf), (int)(word_end-word_start));
                data->InsertChars(data->CursorPos, candidates[0].c_str());
            }
            else
            {
                // Multiple matches. Complete as much as we can, so inputing "C" will complete to "CL" and display "CLEAR" and "CLASSIFY"
                int match_len = (int)(word_end - word_start);
                for (;;)
                {
                    int c = 0;
                    bool all_candidates_matches = true;
                    for (int i = 0; i < candidates.size() && all_candidates_matches; i++)
                        if (i == 0)
                            c = toupper(candidates[i][match_len]);
                        else if (c != toupper(candidates[i][match_len]))
                            all_candidates_matches = false;
                    if (!all_candidates_matches)
                        break;
                    match_len++;
                }
                
                if (match_len > 0)
                {
                    data->DeleteChars((int)(word_start - data->Buf), (int)(word_end-word_start));
                    data->InsertChars(data->CursorPos, candidates[0].c_str(), candidates[0].c_str() + match_len);
                }
            }
            
            break;
        }
    }

    _isInputCompleted = true;

    
    return 0;
}








