//
//  GameResource.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 31..
//
//

#include "GameResource.hpp"
#include "Conditions.hpp"
#include "Actions.hpp"
using namespace realtrick::client;
using namespace cocos2d;

#include "flatbuffers/util.h"

#include <iostream>

bool GameResource::initGMXFile(const std::string& path)
{
    // decode flatbuffers
    std::string loadedData;
    auto ret = flatbuffers::LoadFile(path.c_str(), true, &loadedData);
    if ( ret ) initWithBinary(loadedData.c_str());
    return ret;
}


bool GameResource::initWithBinary(const char* buffer)
{
    const DeadCreator::GMXFile* file = DeadCreator::GetGMXFile(buffer);
    
    
    // 1. default tile
    _defaultTile = file->default_type();
    
    
    // 2. tile size
    _tileWidth = file->tile_size()->width();
    _tileHeight = file->tile_size()->height();
    
    
    // 3. number of tiles
    _numOfTileX = file->number_of_tiles()->x() + DUMMY_TILE_SIZE * 2;
    _numOfTileY = file->number_of_tiles()->y() * 2 + DUMMY_TILE_SIZE * 4;
    
    _worldWidth = _tileWidth * _numOfTileX;
    _worldHeight = _tileHeight * _numOfTileY;
    
    
    // 4. tile infos
    _tileData.resize(_numOfTileY);
    for(int i = 0 ; i < _numOfTileY; ++ i)
    {
        _tileData[i].resize(_numOfTileX);
    }
    
    auto defaultTile = static_cast<TileType>(_defaultTile);
    for(int i = 0; i < _numOfTileY; ++ i)
    {
        for(int j = 0; j < _numOfTileX; ++ j)
        {
            auto pos = indexToPosition(j, i, file->tile_size()->width(), file->tile_size()->height(), DUMMY_TILE_SIZE);
            
            if ( TileType::DIRT == defaultTile ) _tileData[i][j]= Tileset(j, i,"1_" + _to_string(random(1, 3)) + "_1234", pos);
            else if ( TileType::GRASS == defaultTile ) _tileData[i][j]= Tileset(j, i,"2_" + _to_string(random(1, 3)) + "_1234", pos);
            else if ( TileType::WATER == defaultTile ) _tileData[i][j]= Tileset(j, i,"3_" + _to_string(random(1, 3)) + "_1234", pos);
            else if ( TileType::HILL == defaultTile ) _tileData[i][j]= Tileset(j, i,"5_" + _to_string(random(1, 3)) + "_1234", pos);
        }
    }
    
    for ( auto tile = file->tiles()->begin(); tile != file->tiles()->end() ; ++ tile )
    {
        _tileData[tile->indices()->y()][tile->indices()->x()].setNumber(tile->number()->str());
    }
    
    
    // 5. collision regions
    for ( auto poly = file->collision_regions()->begin() ; poly != file->collision_regions()->end() ; ++ poly )
    {
        Polygon p;
        for( auto vert = poly->vertices()->begin(); vert != poly->vertices()->end() ; ++ vert )
        {
            p.pushVertex(Vec2(vert->x(), vert->y()));
        }
        _collisionData.push_back(p);
    }
    
    
    // 5-1. simple collision regions
    for ( auto poly = file->simple_collision_regions()->begin() ; poly != file->simple_collision_regions()->end() ; ++ poly )
    {
        Polygon p;
        for( auto vert = poly->vertices()->begin(); vert != poly->vertices()->end() ; ++ vert )
        {
            p.pushVertex(Vec2(vert->x(), vert->y()));
        }
        _simpleCollisionData.push_back(p);
    }
    
    
    // 6. entities
    for ( auto entity = file->entities()->begin(); entity != file->entities()->end(); ++ entity )
    {
        EntityData data;
        data.id = entity->id();
        data.playerType = static_cast<PlayerType>(entity->player_type());
        data.entityType = static_cast<EntityType>(entity->entity_type());
        data.position.setPoint(entity->pos()->x(), entity->pos()->y());
        data.name = entity->name()->str();
        
        _entities.push_back(data);
    }
    
    
    // 7. cell space size
    _cellWidth = file->cell_space_size()->width();
    _cellHeight = file->cell_space_size()->height();
    
    
    // 8. locations
    for ( auto location = file->locations()->begin(); location != file->locations()->end(); ++location)
    {
        Size size(location->size()->width(), location->size()->height());
        Vec2 pos(location->pos()->x(), location->pos()->y());
        
        _locations.insert(
                          {
                              location->name()->str(),
                              cocos2d::Rect(pos.x, pos.y, size.width * file->tile_size()->width() / 4, size.height * file->tile_size()->height() / 4)
                          });
    }
    
    
    // 9. switchs
    for( auto swc = file->switch_infos()->begin(); swc != file->switch_infos()->end() ; ++ swc)
    {
        int currIndex = swc->index();
        _switchs[currIndex].index = currIndex;
        std::strncpy(_switchs[currIndex].name.data(), swc->name()->c_str(), 100);
        _switchs[currIndex].status = static_cast<SwitchStatus>(swc->status());
    }
    
    
    // 10. triggers
    for ( auto trigger = file->triggers()->begin() ; trigger != file->triggers()->end() ; ++trigger)
    {
        TriggerData data;
        
        // set players
        for(auto index = trigger->players()->begin(); index != trigger->players()->end(); ++index)
        {
            data.players.push_back(*index);
        }
        
        // set conditions
        for(auto cond = trigger->conditions()->begin() ; cond != trigger->conditions()->end(); ++cond)
        {
            auto condType = cond->condition_type();
            switch (condType)
            {
                case DeadCreator::ConditionBase_Bring:
                {
                    auto conditionObject = static_cast<const DeadCreator::Bring*>(cond->condition());
                    
                    auto condition = new ConditionBringData();
                    condition->player = static_cast<PlayerType>(conditionObject->player());
                    condition->approximation = static_cast<ApproximationType>(conditionObject->approximation());
                    condition->number = conditionObject->number();
                    condition->entity = static_cast<EntityType>(conditionObject->entity_type());
                    condition->location = conditionObject->location_name()->str();
                    data.conditions.push_back(condition);
                    
                    break;
                }
                case DeadCreator::ConditionBase_Always:
                {
                    auto condition = new ConditionAlwaysData();
                    data.conditions.push_back(condition);
                    
                    break;
                }
                case DeadCreator::ConditionBase_Never:
                {
                    auto condition = new ConditionNeverData();
                    data.conditions.push_back(condition);
                    
                    break;
                }
                case DeadCreator::ConditionBase_ElapsedTime:
                {
                    auto conditionObject = static_cast<const DeadCreator::ElapsedTime*>(cond->condition());
                    
                    auto condition = new ConditionElapsedTimeData();
                    condition->approximation = static_cast<ApproximationType>(conditionObject->approximation());
                    condition->number = conditionObject->number();
                    data.conditions.push_back(condition);
                    
                    break;
                }
                case DeadCreator::ConditionBase_CountdownTimer:
                {
                    auto conditionObject = static_cast<const DeadCreator::CountdownTimer*>(cond->condition());
                    
                    auto condition = new ConditionCountdownTimerData();
                    condition->approximation = static_cast<ApproximationType>(conditionObject->approximation());
                    condition->number = conditionObject->number();
                    data.conditions.push_back(condition);
                    
                    break;
                }
                case DeadCreator::ConditionBase_Switch:
                {
                    auto conditionObject = static_cast<const DeadCreator::Switch*>(cond->condition());
                    
                    auto condition = new ConditionSwitchData();
                    std::strncpy(condition->info.name.data(), conditionObject->info()->name()->c_str(), 100);
                    condition->info.index = conditionObject->info()->index();
                    condition->info.status = static_cast<SwitchStatus>(conditionObject->info()->status());
                    data.conditions.push_back(condition);
                    
                    break;
                }
                    
                default: { cocos2d::log("invalid condition type"); break;}
            }
        }
        
        // set actions
        for(auto act = trigger->actions()->begin() ; act != trigger->actions()->end(); ++act)
        {
            auto actType = act->action_type();
            switch (actType)
            {
                case DeadCreator::ActionBase_CreateEntity:
                {
                    auto actionObject = static_cast<const struct DeadCreator::CreateEntity*>(act->action());
                    auto action = new ActionCreateEntityData();
                    action->number = actionObject->number();
                    action->entity = static_cast<EntityType>(actionObject->entity_type());
                    action->player = static_cast<PlayerType>(actionObject->player());
                    action->location = actionObject->location_name()->str();
                    data.actions.push_back(action);
                    
                    break;
                }
                case DeadCreator::ActionBase_DisplayText:
                {
                    auto actionObject = static_cast<const DeadCreator::DisplayText*>(act->action());
                    auto action = new ActionDisplayTextData();
                    action->text = actionObject->text()->str();
                    data.actions.push_back(action);
                    
                    break;
                }
                case DeadCreator::ActionBase_PreserveTrigger:
                {
                    auto action = new ActionPreserveTriggerData();
                    data.actions.push_back(action);
                    
                    break;
                }
                case DeadCreator::ActionBase_Victory:
                {
                    auto action = new ActionVictoryData();
                    data.actions.push_back(action);
                    
                    break;
                }
                case DeadCreator::ActionBase_Defeat:
                {
                    auto action = new ActionDefeatData();
                    data.actions.push_back(action);
                    
                    break;
                }
                case DeadCreator::ActionBase_MoveLocation:
                {
                    auto actionObject = static_cast<const DeadCreator::MoveLocation*>(act->action());
                    auto action = new ActionMoveLocationData();
                    action->destLocation = actionObject->dest_location_name()->str();
                    action->entity = static_cast<EntityType>(actionObject->entity_type());
                    action->player = static_cast<PlayerType>(actionObject->player());
                    action->sourceLocation = actionObject->source_location_name()->str();
                    data.actions.push_back(action);
                    
                    break;
                }
                case DeadCreator::ActionBase_KillEntityAtLocation:
                {
                    auto actionObject = static_cast<const DeadCreator::KillEntityAtLocation*>(act->action());
                    auto action = new ActionKillEntityAtLocationData();
                    action->number = actionObject->numberAll();
                    action->entity = static_cast<EntityType>(actionObject->entity_type());
                    action->player = static_cast<PlayerType>(actionObject->player());
                    action->location = actionObject->location_name()->str();
                    data.actions.push_back(action);
                    
                    break;
                }
                case DeadCreator::ActionBase_PlaySound:
                {
                    auto actionObject = static_cast<const DeadCreator::PlaySound*>(act->action());
                    auto action = new ActionPlaySoundData();
                    action->fileName = actionObject->file_name()->str();
                    data.actions.push_back(action);
                    
                    break;
                }
                case DeadCreator::ActionBase_PlaySoundAtLocation:
                {
                    auto actionObject = static_cast<const DeadCreator::PlaySoundAtLocation*>(act->action());
                    auto action = new ActionPlaySoundAtLocationData();
                    action->fileName = actionObject->file_name()->str();
                    action->location = actionObject->location_name()->str();
                    data.actions.push_back(action);
                    
                    break;
                }
                case DeadCreator::ActionBase_MoveEntity:
                {
                    auto actionObject = static_cast<const DeadCreator::MoveEntity*>(act->action());
                    auto action = new ActionMoveEntityData();
                    action->number = actionObject->numberAll();
                    action->entity = static_cast<EntityType>(actionObject->entity_type());
                    action->player = static_cast<PlayerType>(actionObject->player());
                    action->sourceLocation = actionObject->src_location_name()->str();
                    action->destLocation = actionObject->dst_location_name()->str();
                    data.actions.push_back(action);
                    
                    break;
                }
                case DeadCreator::ActionBase_PauseGame:
                {
                    auto action = new ActionPauseGameData();
                    data.actions.push_back(action);
                    
                    break;
                }
                case DeadCreator::ActionBase_ResumeGame:
                {
                    auto action = new ActionResumeGameData();
                    data.actions.push_back(action);
                    
                    break;
                }
                case DeadCreator::ActionBase_SetCountdownTimer:
                {
                    auto actionObject = static_cast<const DeadCreator::SetCountdownTimer*>(act->action());
                    auto action = new ActionSetCountdownTimerData();
                    action->arithmetical = static_cast<ArithmeticalType>(actionObject->arithmetical());
                    action->number = actionObject->number();
                    data.actions.push_back(action);

                    break;
                }
                case DeadCreator::ActionBase_SetSwitch:
                {
                    auto actionObject = static_cast<const DeadCreator::SetSwitch*>(act->action());
                    auto action = new ActionSetSwitchData();
                    std::strncpy(action->info.name.data(), actionObject->info()->name()->c_str(), 100);
                    action->info.index = actionObject->info()->index();
                    action->info.status = static_cast<SwitchStatus2>(actionObject->info()->status());
                    data.actions.push_back(action);
                    
                    break;
                }
                    
                default: { cocos2d::log("invalid action type"); break;}
            }
        }
        
        _triggers.push_back(data);
    }
    
    
    // 11. force infos
    strncpy(_forces[0].name.data(), file->force1_info()->name()->c_str(), 20);
    _forces[0].isAlly = file->force1_info()->is_ally();
    _forces[0].isVision = file->force1_info()->is_vision();
    
    strncpy(_forces[1].name.data(), file->force2_info()->name()->c_str(), 20);
    _forces[1].isAlly = file->force2_info()->is_ally();
    _forces[1].isVision = file->force2_info()->is_vision();
    
    strncpy(_forces[2].name.data(), file->force3_info()->name()->c_str(), 20);
    _forces[2].isAlly = file->force3_info()->is_ally();
    _forces[2].isVision = file->force3_info()->is_vision();
    
    strncpy(_forces[3].name.data(), file->force4_info()->name()->c_str(), 20);
    _forces[3].isAlly = file->force4_info()->is_ally();
    _forces[3].isVision = file->force4_info()->is_vision();
    
    
    // 12. player infos
    std::vector<int> forcePlayers[4];
    int iter = 1;
    for ( auto info = file->playerInfos()->begin(); info != file->playerInfos()->end() ; ++ info )
    {
		if ( static_cast<Owner>(info->owner()) == Owner::UNUSED ) continue; 

        _playerInfos[iter] = PlayerInfo(static_cast<PlayerType>(info->player()),
                                     static_cast<Force>(info->force()),
                                     static_cast<Owner>(info->owner()));
        

        forcePlayers[info->force()].push_back(info->player());
     
        ++iter;
    }
    
    for (int i = 1 ; i <= 8; ++ i)
    {
        int force = static_cast<int>(_playerInfos[i].force);
        for( auto player : forcePlayers[force] )
        {
			_playerInfos[i].isAllyWith[player] = _forces[force].isAlly;
            _playerInfos[i].isSharedVision[player] = _forces[force].isVision;
        }
        
        _playerInfos[i].isAllyWith[i] = true;
        _playerInfos[i].isSharedVision[i] = true;
    }
    
    return true;
}


void GameResource::updateLocation(const std::string& key, const cocos2d::Rect& rect)
{
    // location is update only. (can't create new one)
    if ( _locations.count(key) == 0)
        throw std::runtime_error(cocos2d::StringUtils::format("%s(key) is not exist location", key.c_str()));
    
    _locations[key] = rect;
}









