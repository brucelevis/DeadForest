//
//  main.cpp
//  flatbuffer_master
//
//  Created by mac on 2016. 7. 7..
//  Copyright © 2016년 realtrick. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#include "monster_generated.h"
#include "flatbuffers/util.h"
using namespace MyGame::Sample;

int main()
{
    
    flatbuffers::FlatBufferBuilder builder;
    auto weapon_one_name = builder.CreateString("Sword");
    short weapon_one_damage = 3;
    
    auto weapon_two_name = builder.CreateString("Axe");
    short weapon_two_damage = 5;
    
    auto sword = CreateWeapon(builder, weapon_one_name, weapon_one_damage);
    auto axe = CreateWeapon(builder, weapon_two_name, weapon_two_damage);
    
    auto name = builder.CreateString("Orc");
    
    std::vector<flatbuffers::Offset<Weapon>> weapons_vector;
    weapons_vector.push_back(sword);
    weapons_vector.push_back(axe);
    auto weapons = builder.CreateVector(weapons_vector);
    
    auto pos = Vec3(1.0f, 2.0f, 3.0f);
    int hp = 300;
    int mana = 150;
    
    MonsterBuilder monster_builder(builder);
    monster_builder.add_pos(&pos);
    monster_builder.add_hp(hp);
    monster_builder.add_mana(mana);
    monster_builder.add_name(name);
    monster_builder.add_color(Color_Red);
    monster_builder.add_weapons(weapons);
    auto orc = monster_builder.Finish();
    builder.Finish(orc);
    
    flatbuffers::SaveFile("/Users/jun/Desktop/untitled.txt",
                          reinterpret_cast<const char*>(builder.GetBufferPointer()),
                          builder.GetSize(),
                          false);
    
    std::string loaded_file;
    int ret = flatbuffers::LoadFile("/Users/jun/Desktop/untitled.txt", false, &loaded_file);
    if ( ret )
    {
        auto monster = GetMonster(loaded_file.c_str());
        cout << monster->hp() << endl;
        cout << monster->mana() << endl;
        cout << monster->pos()->x() << " " << monster->pos()->y() << " " << monster->pos()->z() << endl;
        auto weapons = monster->weapons();
        for (auto iter2 = weapons->begin() ; iter2 != weapons->end() ; ++iter2)
        {
            cout << iter2->name()->str() << " " << iter2->damage() << endl;
        }
    }
    
}









