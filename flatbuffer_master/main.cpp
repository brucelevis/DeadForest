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
    unsigned char treasure[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto inventory = builder.CreateVector(treasure, 10);
    
    std::vector<flatbuffers::Offset<Weapon>> weapons_vector;
    weapons_vector.push_back(sword);
    weapons_vector.push_back(axe);
    auto weapons = builder.CreateVector(weapons_vector);
    
    // test
    
}