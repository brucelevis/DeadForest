//
//  PropertyEditLayer.hpp
//  DeadCreator
//
//  Created by mac on 2016. 11. 16..
//
//

#pragma once


#include <algorithm>

#include "cocos2d.h"

#include "realtrick/imgui/imgui.h"
#include "realtrick/imgui/imgui_internal.h"


namespace realtrick
{
    namespace editor
    {
        
        class GMXLayer;
        
        class PropertyEditLayer : public cocos2d::Node
        {
            
        public:
            
            struct HumanProperty
            {
                int hp = 100;
                int walkSpeed = 75;
                int runSpeed = 150;
            };
            
            struct WeaponProperty
            {
                int damage = 0;
                int range = 0;
            };
            
        public:
            
            explicit PropertyEditLayer(GMXLayer& layer) :
            _gmxLayer(layer)
            {
            }
            
            virtual ~PropertyEditLayer() = default;
            static PropertyEditLayer* create(GMXLayer& layer)
            {
                auto ret = new (std::nothrow) PropertyEditLayer(layer);
                if ( ret && ret->init() )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_FREE(ret);
                return nullptr;
            }
            
            void showLayer(bool& opened)
            {
                ImGuiContext& g = *GImGui;
                ImGui::SetNextWindowPos(ImVec2((g.IO.DisplaySize.x - 600) / 2,
                                               (g.IO.DisplaySize.y - 400) / 2), ImGuiSetCond_FirstUseEver);
                ImGui::SetNextWindowSize(ImVec2(600, 400));
                ImGui::OpenPopup("Edit Property");
                if (ImGui::BeginPopupModal("Edit Property", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    ImGui::Separator();
                    ImGui::BeginChild("##dummy", ImVec2(0, 335), false);
                    
                    if ( ImGui::TreeNode("Humans") )
                    {
                        if ( ImGui::TreeNode("Sheriff") )
                        {
                            static int hp = 100;
                            static int walkSpeed = 75;
                            static int runSpeed = 150;
                            
                            if (ImGui::InputInt("hp", &hp))
                            {
                                hp = std::min(std::max(1, hp), 9999);
                            }
                            if (ImGui::InputInt("walk speed", &walkSpeed))
                            {
                                walkSpeed = std::min(std::max(1, walkSpeed), 999);
                            }
                            if (ImGui::InputInt("run speed", &runSpeed))
                            {
                                runSpeed = std::min(std::max(1, runSpeed), 999);
                            }
                            
                            ImGui::TreePop();
                        }
                        if ( ImGui::TreeNode("Zombie 1") )
                        {
                            static int hp = 100;
                            static int walkSpeed = 75;
                            static int runSpeed = 150;
                            
                            if (ImGui::InputInt("hp", &hp))
                            {
                                hp = std::min(std::max(1, hp), 9999);
                            }
                            if (ImGui::InputInt("walk speed", &walkSpeed))
                            {
                                walkSpeed = std::min(std::max(1, walkSpeed), 999);
                            }
                            if (ImGui::InputInt("run speed", &runSpeed))
                            {
                                runSpeed = std::min(std::max(1, runSpeed), 999);
                            }
                            ImGui::TreePop();
                        }
                        if ( ImGui::TreeNode("Zombie 2") )
                        {
                            static int hp = 100;
                            static int walkSpeed = 75;
                            static int runSpeed = 150;
                            
                            if (ImGui::InputInt("hp", &hp))
                            {
                                hp = std::min(std::max(1, hp), 9999);
                            }
                            if (ImGui::InputInt("walk speed", &walkSpeed))
                            {
                                walkSpeed = std::min(std::max(1, walkSpeed), 999);
                            }
                            if (ImGui::InputInt("run speed", &runSpeed))
                            {
                                runSpeed = std::min(std::max(1, runSpeed), 999);
                            }
                            ImGui::TreePop();
                        }
                        if ( ImGui::TreeNode("Zombie 3") )
                        {
                            static int hp = 100;
                            static int walkSpeed = 75;
                            static int runSpeed = 150;
                            
                            if (ImGui::InputInt("hp", &hp))
                            {
                                hp = std::min(std::max(1, hp), 9999);
                            }
                            if (ImGui::InputInt("walk speed", &walkSpeed))
                            {
                                walkSpeed = std::min(std::max(1, walkSpeed), 999);
                            }
                            if (ImGui::InputInt("run speed", &runSpeed))
                            {
                                runSpeed = std::min(std::max(1, runSpeed), 999);
                            }
                            ImGui::TreePop();
                        }
                        
                        ImGui::TreePop();
                    }
                    if ( ImGui::TreeNode("Weapons") )
                    {
                        if ( ImGui::TreeNode("Axe") )
                        {
                            static int damage = 100;
                            static int range = 100;
                            
                            if (ImGui::InputInt("damage", &damage))
                            {
                                damage = std::min(std::max(1, damage), 9999);
                            }
                            if (ImGui::InputInt("range", &range))
                            {
                                range = std::min(std::max(1, range), 9999);
                            }
                            
                            ImGui::TreePop();
                        }
                        if ( ImGui::TreeNode("Glock17") )
                        {
                            static int damage = 100;
                            static int range = 100;
                            
                            if (ImGui::InputInt("damage", &damage))
                            {
                                damage = std::min(std::max(1, damage), 9999);
                            }
                            if (ImGui::InputInt("range", &range))
                            {
                                range = std::min(std::max(1, range), 9999);
                            }
                            
                            ImGui::TreePop();
                        }
                        if ( ImGui::TreeNode("M16A2") )
                        {
                            static int damage = 100;
                            static int range = 100;
                            
                            if (ImGui::InputInt("damage", &damage))
                            {
                                damage = std::min(std::max(1, damage), 9999);
                            }
                            if (ImGui::InputInt("range", &range))
                            {
                                range = std::min(std::max(1, range), 9999);
                            }
                            
                            ImGui::TreePop();
                        }
                        if ( ImGui::TreeNode("M1897") )
                        {
                            static int damage = 100;
                            static int range = 100;
                            
                            if (ImGui::InputInt("damage", &damage))
                            {
                                damage = std::min(std::max(1, damage), 9999);
                            }
                            if (ImGui::InputInt("range", &range))
                            {
                                range = std::min(std::max(1, range), 9999);
                            }
                            
                            ImGui::TreePop();
                        }
                        
                        ImGui::TreePop();
                    }
                
                    ImGui::EndChild();
                    
                    ImGui::Separator();
                    if (ImGui::Button("Ok", ImVec2(100, 20)))
                    {
                        
                        
                        closeWindow();
                        opened = false;
                    }
                    
                    ImGui::SameLine();
                    if (ImGui::Button("Cancel", ImVec2(100, 20)))
                    {
                        closeWindow();
                        opened = false;
                    }
                    
                    ImGui::EndPopup();
                }
                
            }
            
            void saveToFile()
            {
                
            }
            
            void closeWindow()
            {
                ImGui::CloseCurrentPopup();
            }
            
        private:
            
            HumanProperty _playerProperty;
            HumanProperty _zombie1Property;
            HumanProperty _zombie2Property;
            HumanProperty _zombie3Property;
            
            WeaponProperty _axeProperty;
            WeaponProperty _glock17Property;
            WeaponProperty _m16a2Property;
            WeaponProperty _m1897Property;
            
            GMXLayer& _gmxLayer;
            
        };
        
    }
}









