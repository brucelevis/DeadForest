//
//  main.cpp
//  flatbuffer_master
//
//  Created by mac on 2016. 7. 7..
//  Copyright © 2016년 realtrick. All rights reserved.
//

#include <iostream>
#include <vector>
using namespace std;

#include "monster_generated.h"
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
    unsigned char treasure[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto inventory = builder.CreateVector(treasure, 10);
    
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
    monster_builder.add_inventory(inventory);
    monster_builder.add_color(Color_Red);
    monster_builder.add_weapons(weapons);
    auto orc = monster_builder.Finish();
    builder.Finish(orc);
 
    {
        auto buffer_pointer = builder.GetBufferPointer();
        auto monster = GetMonster(buffer_pointer);
        
        cout << monster->hp() << endl;
        cout << monster->mana() << endl;
        cout << monster->pos()->x() << " " << monster->pos()->y() << " " << monster->pos()->z() << endl;
        
        auto inv = monster->inventory();
        auto inv_len = inv->Length();
        for(auto i = 0; i < inv_len; ++ i)
        {
            cout << inv->Get(i) << " ";
        }
        cout << endl;
        
        auto weapons = monster->weapons();
        auto weapon_len = weapons->Length();
        auto second_weapon_name = weapons->Get(1)->name()->str();
        auto second_weapon_damage = weapons->Get(1)->damage();
        
        cout << weapon_len << endl;
        cout << second_weapon_name << endl;
        cout << second_weapon_damage << endl;
    }
    
}









