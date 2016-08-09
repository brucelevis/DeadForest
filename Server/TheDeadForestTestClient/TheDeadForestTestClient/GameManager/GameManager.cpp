
#include "GameManager.hpp"

using namespace realtrick;


void GameManager::update(float dt)
{
	for (const auto& entity : _entities)
	{
		entity.second->update(dt);
	}
    
}

void GameManager::loadGameMap(const char* fileName)
{
	if (_gameMap != nullptr)
	{
		throw std::runtime_error("<GameManager::loadGameMap> GameMap is already exist.");
    }
    
	_cellSpace = new CellSpacePartition(Prm.getValueAsInt("worldWidth"),
		Prm.getValueAsInt("worldHeight"),
		Prm.getValueAsInt("cellWidth"),
		Prm.getValueAsInt("cellHeight"),
		Prm.getValueAsInt("maxEntity"));

	_gameMap = GameMap::create(this, fileName);

	std::vector<Vec2> startingPoints = getGameMap()->getStartingLocationData();

	const std::map<int, realtrick::Session*>& sessionGroup = _room->getPlayers();


	int i = 0;
	for (auto tmpPlayer = sessionGroup.begin(); tmpPlayer != sessionGroup.end(); ++tmpPlayer)
	{
		EntityHuman* human = EntityHuman::create(this);
		human->setWorldPosition(startingPoints[i++]);
		int vId = getNextValidID();
		_room->setIdConvertGroup(tmpPlayer->second->getPid(), vId);
		addDynamicEntity(human, Z_ORDER_HUMAN, vId);
		cout << "------------------------------------------" << endl;
		cout << "vid: " << vId << "pid: " << tmpPlayer->second->getPid() << "nick: " << tmpPlayer->second->getNickname() << endl;
	}

	
	for (auto session = sessionGroup.begin(); session != sessionGroup.end(); ++session)
	{
		Session* currSession = session->second;
		int currID = _room->getPidToVid(currSession->getPid());

		flatbuffers::FlatBufferBuilder builder;
		std::vector<flatbuffers::Offset<fpacket::Human>> humanVec;


		for (auto entity : _entities)
		{
			auto otherID = entity.second->getTag();

			if (currID == entity.second->getTag()) continue;

			auto h = fpacket::CreateHuman(builder,
				otherID,
				builder.CreateString(_room->getPlayer(_room->getVidToPid(otherID))->getNickname()),
				entity.second->getWorldPosition().x,
				entity.second->getWorldPosition().y);
			cout << "vid: " << otherID << "nick: " << _room->getPlayer(_room->getVidToPid(otherID))->getNickname() << endl;
			humanVec.push_back(h);

		}
				
		auto own = fpacket::CreateHuman(builder, 
			currID, 
			builder.CreateString(currSession->getNickname()), 
			getEntityFromID(currID)->getWorldPosition().x, 
			getEntityFromID(currID)->getWorldPosition().y);

		auto obj = fpacket::CreatePacketFirstPlayerInfos(builder, own, builder.CreateVector(humanVec));
		builder.Finish(obj);

		uint8_t* body = builder.GetBufferPointer();
		uint32_t size = builder.GetSize();

		FlatbufferPacket* packet = new FlatbufferPacket();
		packet->encode(body, size, PacketType::FISRT_PLAYER_INFOS);

		_room->getServer()->sendFisrtPlayerInfos(currSession, packet);
	}
}

EntityBase* GameManager::getEntityFromID(int ID)
{
	auto iter = _entities.find(ID);
	if (iter != _entities.end())
	{
		return iter->second;
	}
	else
	{
		return nullptr;
	}
}

void GameManager::addDynamicEntity(EntityBase* entity, int zOrder, int id)
{
	auto iter = _entities.find(id);
	if (iter != _entities.end())
	{
		throw std::runtime_error("<GameManager::registEntity> ID is already exist.");
	}

	entity->setTag(id);
	_entities.insert({ id, entity });
	_cellSpace->addEntity(entity);

}

void GameManager::removeEntity(int id)
{
	auto iter = _entities.find(id);
	if (iter == _entities.end())
	{
		throw std::runtime_error("<GameManager::removeEntity> ID is not exist.");
	}
	delete(iter->second);
	iter->second = nullptr;
	_entities.erase(iter);
	_cellSpace->removeEntityFromCell(iter->second);
}


void GameManager::clear()
{
	_entities.clear();
	delete(_cellSpace);
	_cellSpace = nullptr;
}


std::list<EntityBase*> GameManager::getNeighborsOnMove(const Vec2& position, float speed) const
{
	std::list<EntityBase*> ret;
	std::vector<int> cellIndices = _cellSpace->getNeighborCells(position);
	for (const int idx : cellIndices)
	{
		const Cell& currCell = _cellSpace->getCell(idx);
		if (currCell.boundingBox.intersectsRect(Rect(position.x - speed / 2, position.y - speed / 2, speed, speed)))
		{
			for (const auto &entity : currCell.members)
			{
				ret.push_back(entity);
			}

		}
	}
	return std::move(ret);
}

std::list<EntityBase*> GameManager::getNeighborsOnAttack(const Vec2& position, const Vec2& dir, float range) const
{
	std::list<EntityBase*> ret;
	std::vector<int> cellIndices = _cellSpace->getNeighborCellsNotCurrent(position);
	for (const int idx : cellIndices)
	{
		const Cell& currCell = _cellSpace->getCell(idx);
		if (physics::intersect(geometry::Rect(currCell.boundingBox.origin.x,
			currCell.boundingBox.origin.y,
			currCell.boundingBox.size.width,
			currCell.boundingBox.size.height), geometry::Segment(position, position + dir * range)))
		{
			for (const auto &entity : currCell.members)
			{
				ret.push_back(entity);
			}
		}
	}

	const Cell& myCell = _cellSpace->getCell(_cellSpace->positionToIndex(position));
	for (const auto& entity : myCell.members)
	{
		ret.push_back(entity);
	}

	return std::move(ret);
}

std::vector<geometry::Polygon> GameManager::getNeighborWalls(const Vec2& position, float speed) const
{
	std::vector<geometry::Polygon> ret;
	std::vector<int> cellIndices = _cellSpace->getNeighborCells(position);
	for (const int idx : cellIndices)
	{
		const Cell& currCell = _cellSpace->getCell(idx);
		if (currCell.boundingBox.intersectsRect(Rect(position.x - speed / 2, position.y - speed / 2, speed, speed)))
		{
			for (const auto &wall : currCell.walls)
			{
				ret.push_back(wall);
			}
		}
	}
	return std::move(ret);
}


std::vector<geometry::Polygon> GameManager::getNeighborWalls(const Vec2& position, const Size screenSize) const
{
	std::vector<geometry::Polygon> ret;
	std::vector<int> cellIndices = _cellSpace->getNeighborCells(position);
	for (const int idx : cellIndices)
	{
		const Cell& currCell = _cellSpace->getCell(idx);
		if (currCell.boundingBox.intersectsRect(Rect(position - screenSize / 2, screenSize)))
		{
			for (const auto &wall : currCell.walls)
			{
				ret.push_back(wall);
			}
		}
	}
	return std::move(ret);
}


std::vector<geometry::Polygon> GameManager::getNeighborWalls(const Vec2& position, const geometry::Segment& ray) const
{
	std::vector<geometry::Polygon> ret;
	std::vector<int> cellIndices = _cellSpace->getNeighborCellsNotCurrent(position);
	for (const int idx : cellIndices)
	{
		const Cell& currCell = _cellSpace->getCell(idx);
		if (physics::intersect(geometry::Rect(currCell.boundingBox.origin,
			currCell.boundingBox.size.width, 
			currCell.boundingBox.size.height), ray))
		{
			for (const auto &wall : currCell.walls)
			{
				ret.push_back(wall);
			}
		}
	}

	const Cell& myCell = _cellSpace->getCell(_cellSpace->positionToIndex(position));
	for (const auto& wall : myCell.walls)
	{
		ret.push_back(wall);
	}

	return std::move(ret);
}

void GameManager::moveJoystick(FlatbufferPacket *packet, int pid)
{
	cout << "[moveJoystick]GameManager" << endl;
    
    auto move = fpacket::GetPacketMoveJoystick(packet->body());
    
    auto entity = static_cast <EntityHuman*>(getEntityFromID(_room->getPidToVid(pid)));
    
    Vec2 pos = Vec2(move->dir_x(), move->dir_y());
    entity->setMoving(pos);
    
    if(move->is_touched()){
        entity->addInputMask(HumanBehaviorType::RUN);
    }else{
        entity->removeInputMask(HumanBehaviorType::RUN);
    }
    
	const std::map<int, realtrick::Session*>& sessionGroup = _room->getPlayers();

	for (auto session : sessionGroup)
	{
		Session* currSession = session.second;
		if (currSession->getPid() != pid) {
			_room->getServer()->sendMoveJoystick(currSession, packet);
		}
	}
}


void GameManager::setLoadGamePlayer(int vid)
{
	auto entity = static_cast <EntityHuman*>(getEntityFromID(vid));
    if(entity != nullptr){
        entity->setIsLoad(true);

        if (isAllReady()) {
            sendGameStart();
        }
    }
}

void GameManager::sendGameStart()
{
	_room->setRoomState(static_cast <int> (RoomState::INGAME));

	const std::map<int, realtrick::Session*>& sessionGroup = _room->getPlayers();

	for (auto session : sessionGroup)
	{
		Session* currSession = session.second;
		_room->getServer()->sendGameStart(currSession);
        _room->updateGame();
	}
}

bool GameManager::isAllReady()
{
	bool isAllReady = true;

	for (auto entity : _entities)
	{
		auto hEntity = static_cast <EntityHuman*>(entity.second);
		if (!(hEntity->getIsLoad()))
		{
			return false;
		}
	}
	return isAllReady;
}