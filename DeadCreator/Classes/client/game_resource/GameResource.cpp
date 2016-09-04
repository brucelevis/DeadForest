//
//  GameResource.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 31..
//
//

#include "GameResource.hpp"
#include "Conditions.hpp"
#include "Actions.hpp"
using namespace realtrick::client;
using namespace cocos2d;

#include "util.h"


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
    
    // 6. entities
    for ( auto entity = file->entities()->begin(); entity != file->entities()->end(); ++ entity )
    {
        EntityData data;
        data.id = entity->id();
        data.playerType = static_cast<PlayerType>(entity->player_type());
        data.entityType = static_cast<EntityType>(entity->entity_type());
        data.position.setPoint(entity->pos()->x(), entity->pos()->y());
        
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
    
    
    // 9. triggers
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
                    
                default: { cocos2d::log("invalid condition type"); break;}
            }
        }
        // set actions
        for(auto act = trigger->actions()->begin() ; act != trigger->actions()->end(); ++act)
        {
            auto actType = act->action_type();
            switch (actType)
            {
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
                default: { cocos2d::log("invalid action type"); break;}
            }
        }
        
        _triggers.push_back(data);
    }
    
    // 10. player infos
    int i = 1;
    for ( auto info = file->playerInfos()->begin(); info != file->playerInfos()->end() ; ++ info )
    {
        
        _playerInfos[i] = PlayerInfo(static_cast<PlayerType>(info->player()),
                                     static_cast<Force>(info->force()),
                                     static_cast<Owner>(info->owner()));
        ++i;
    }
    
    return true;
}


void GameResource::updateLocation(const std::string& key, const cocos2d::Rect& rect)
{
    // location is update only. (can't create new one)
    if ( _locations.count(key) == 0) throw std::runtime_error(cocos2d::StringUtils::format("%s(key) is not exist location", key.c_str()));
    
    _locations[key] = rect;
}









