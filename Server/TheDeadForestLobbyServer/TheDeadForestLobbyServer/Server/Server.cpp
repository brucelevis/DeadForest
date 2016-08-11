
#include "Server.h"

using namespace realtrick;
using namespace boost;
using namespace std;


#pragma mark - getter/setter Method

void Server::setLastUserId()
{
    _dbManager->init();
    _dbManager->setLastUserId(_lastUserId);
}

void Server::getLastUserId()
{
    _dbManager->init();
    _dbManager->getLastUserId(_lastUserId);
    
}

Session* Server::getSession(int id)
{
    std::map<int, realtrick::Session*>::iterator findSession = _sessionGroup.find(id);
    
    if (_sessionGroup.end() != findSession) {
        return findSession->second;
    }
    
    return NULL;
}

void Server::deleteSession(int id)
{
    cout << "deleteSession" << endl;
    std::map<int, realtrick::Session*>::iterator findSession = _sessionGroup.find(id);
    
    if (_sessionGroup.end() != findSession) {
        Session* delSession = findSession->second;
        delete delSession;
        _sessionGroup.erase(findSession);
    }
}

Room* Server::getRoom(int roomId)
{
    cout << "getRoom:" << roomId << endl;
    std::map<int, realtrick::Room*>::iterator findRoom = _roomGroup.find(roomId);
    
    if (_roomGroup.end() != findRoom) {
        return findRoom->second;
    }
    
    return NULL;
}

void Server::deleteRoom(int roomId)
{
    cout << "deleteRoom:" << roomId << endl;
    std::map<int, realtrick::Room*>::iterator delRoom = _roomGroup.find(roomId);
    
    if (_roomGroup.end() != delRoom) {
        delete delRoom->second;
        _roomGroup.erase(delRoom);
    }
}

boost::asio::io_service* Server::getIOService()
{
    return &_io;
}

#pragma mark - Timer Method

unsigned int Server::getTickCount()
{
    struct timeval gettick;
    gettimeofday(&gettick, NULL);
    
    return static_cast<unsigned int>(gettick.tv_sec*1000 + gettick.tv_usec/1000);
}

void Server::enqueueTimer(int id, unsigned int time, int type)
{
    _timerQueue.push(eventToken{ id, time, type });
}

#pragma mark - Packet en/dequeue Method

void Server::enqueuePacket(Package* package)
{
    while(!_packetQueue.push(package));
}

void Server::dequeuePacket(Package*& package)
{
    while (!_packetQueue.pop(package));
}

#pragma mark - Send Packet Method

void Server::sendSuccessLogin(Session* session, int pid, string nickName)
{
    cout << endl;
    cout << "==========sendSuccessLogin==========" << endl;
    flatbuffers::FlatBufferBuilder builder;
    flatbuffers::Offset<flatbuffers::String> nick = builder.CreateString(nickName);
    
    fpacket::PacketLoginSuccessBuilder packetLoginSuccessBuilder(builder);
    packetLoginSuccessBuilder.add_pid(pid);
    packetLoginSuccessBuilder.add_id(nick);
    auto obj = packetLoginSuccessBuilder.Finish();
    builder.Finish(obj);
    
    uint8_t* body = builder.GetBufferPointer();
    uint32_t size = builder.GetSize();
    
    FlatbufferPacket* packet = new FlatbufferPacket();
    packet->encode(body, size, PacketType::LOGIN_SUCCESS);
    
    session->write(packet);
}

void Server::sendErrorCode(Session* session, int errorCode)
{
    cout << endl;
    cout << "==========sendErrorCode==========" << endl;
    cout << "=>sendErrorCode :" << errorCode << endl;
    
    flatbuffers::FlatBufferBuilder builder;
    fpacket::PacketLoginFailBuilder packetLoginFailBuilder(builder);
    packetLoginFailBuilder.add_error_code(errorCode);
    auto obj = packetLoginFailBuilder.Finish();
    builder.Finish(obj);
    
    uint8_t* body = builder.GetBufferPointer();
    uint32_t size = builder.GetSize();
    
    FlatbufferPacket* packet = new FlatbufferPacket();
    packet->encode(body, size, PacketType::LOGIN_FAIL);
    
    session->write(packet);
}

void Server::sendSuccessSearchGame(Session* session)
{
    cout << endl;
    cout << "==========sendSuccessSearchGame==========" << endl;
    cout << "=>id:" << session ->getPid() << endl;
    
    FlatbufferPacket* packet = new FlatbufferPacket();
    packet->encode(nullptr, 0, PacketType::SUCCESS_SEARCH_GAME);
    
    session->write(packet);
}

void Server::sendFailSearchGame(Session* session)
{
    cout << endl;
    cout << "==========sendFailSearchGame==========" << endl;
    
    FlatbufferPacket* packet = new FlatbufferPacket();
    packet->encode(nullptr, 0, PacketType::FAIL_SEARCH_GAME);
    
    session->write(packet);
}

void Server::sendFisrtPlayerInfos(Session* session, FlatbufferPacket* packet)
{
    cout << endl;
    cout << "==========sendFisrtPlayerInfos==========" << endl;
    
    session->write(packet);
    
}

void Server::sendGameStart(Session* session)
{
    cout << endl;
    cout << "==========sendGameStart==========" << endl;
    
    FlatbufferPacket* packet = new FlatbufferPacket();
    packet->encode(nullptr, 0, PacketType::GAME_START);
    
    session->write(packet);
}

void Server::sendMoveJoystick(Session* session, FlatbufferPacket *packet)
{
    cout << endl;
    cout << "==========sendMoveJoystick==========" << endl;
    
    
    auto move = fpacket::GetPacketMoveJoystick(packet->body());
    
    cout << "id:" << move->id() << endl;
    cout << "dir_x:" << move->dir_x() << endl;
    cout << "dir_y:" << move->dir_y() << endl;
    cout << "is_touched:" << move->is_touched() << endl;
    
    cout << "=>nickname : " << session->getNickname() << endl;
    session->write(packet);
}

#pragma mark - Packet Process Method


void Server::loginProcess(Package* package)
{
    cout << endl;
    cout << "==========loginProcess==========" << endl;
    FlatbufferPacket *packet = package->packet;
    
    auto loginPacket = fpacket::GetPacketLogin(packet->body());
    
    if (loginPacket->pid() == -1) {
        cout << "=>New User Connected" << endl;
        
        int pid = _lastUserId++;
        string nickName = "guest" + to_string(pid);
        
        _dbManager->init();
        int result = _dbManager->createGeustID(pid, nickName);
        if (result == 0) {
            cout << "=>New User Login Success" << endl;
            package->session->setPid(pid);
            package->session->setNickname(nickName);
            sendSuccessLogin(package->session, pid, nickName);
            setLastUserId();
            delete package->packet;
            package->packet = nullptr;
            delete package;
            package = nullptr;
            return;
        }
        
        cout << "=>New User Login error:" << result << endl;
        //ø°∑Ø √≥∏Æ: æ∆¿Ãµ ª˝º∫ Ω«∆–(1)
        sendErrorCode(package->session, result);
        delete package->packet;
        package->packet = nullptr;
        delete package;
        package = nullptr;
        return;
    }
    else {
        cout << "=> User Connected" << endl;
        string nickName = "";
        
        _dbManager->init();
        int result = _dbManager->checkIDExist(loginPacket->pid(), nickName);
        if (result == 0) {
            //∑Œ±◊¿Œ √≥∏Æ
            cout << "=>Exist User Login Success" << endl;
            package->session->setPid(loginPacket->pid());
            package->session->setNickname(nickName);
            sendSuccessLogin(package->session, loginPacket->pid(), nickName);
            delete package->packet;
            package->packet = nullptr;
            delete package;
            package = nullptr;
            return;
        }
        //ø°∑Ø √≥∏Æ: æ∆¿Ãµ æ¯¿Ω(101), DBø¿∑˘(1)
        cout << "=>Exist User Login error:" << result << endl;
        sendErrorCode(package->session, result);
        delete package->packet;
        package->packet = nullptr;
        delete package;
        package = nullptr;
        return;
    }
}

void Server::signupProcess(Package* package)
{
    cout << endl;
    cout << "==========signupProcess==========" << endl;
    
    FlatbufferPacket *packet = package->packet;
    
    auto signPacket = fpacket::GetPacketSignup(packet->body());
    
    cout << "=>pid:" << signPacket->pid() << " id:" << signPacket->id()->c_str() << endl;
    
    _dbManager->init();
    //¥–≥◊¿” ¡∏¿Áø©∫Œ »Æ¿Œ
    int result = _dbManager->checkNicknameExist(signPacket->id()->c_str());
    
    cout << "=>result:" << result << endl;
    if (result == 0) {
        //¥–≥◊¿” æ˜µ•¿Ã∆Æ
        _dbManager->init();
        result = _dbManager->setUserNickname(signPacket->pid(), signPacket->id()->c_str());
        
        cout << "=>result2:" << result << endl;
        if (result == 0) {
            //∑Œ±◊¿Œ √≥∏Æ
            sendSuccessLogin(package->session, signPacket->pid(), signPacket->id()->c_str());
            delete package->packet;
            package->packet = nullptr;
            delete package;
            package = nullptr;
            return;
        }
        //ø°∑Ø √≥∏Æ: DBø¿∑˘(1)
        sendErrorCode(package->session, result);
        delete package->packet;
        package->packet = nullptr;
        delete package;
        package = nullptr;
        return;
    }
    
    //ø°∑Ø √≥∏Æ: ¥–≥◊¿”¿Ã ¿ÃπÃ ¡∏¿Á(101), DBø¿∑˘(1)
    sendErrorCode(package->session, result);
    delete package->packet;
    package->packet = nullptr;
    delete package;
    package = nullptr;
    return;
}

void Server::searchRoomProcess(Package* package)
{
    cout << endl;
    cout << "==========searchRoomProcess==========" << endl;
    if (package->session->getRoomid() != -1)
    {
        sendFailSearchGame(package->session);
        delete package->packet;
        package->packet = nullptr;
        delete package;
        package = nullptr;
        return;
    }
    
    FlatbufferPacket *packet = package->packet;
    
    auto doSearchGamePacket = fpacket::GetPacketDoSearchGame(packet->body());
    
    cout << "=>mode:" << doSearchGamePacket->mode() << endl;
    
    bool isExistRoom = false;
    
    for (auto tmpRoom = _roomGroup.begin(); tmpRoom != _roomGroup.end(); ++tmpRoom)
    {
        if ( tmpRoom->second->getRoomMode() == doSearchGamePacket->mode()
            && tmpRoom->second->getRoomState() == static_cast<int> (RoomState::WAIT) )
        {
            if (tmpRoom->second->setPlayer(package->session))
            {
                cout << "=>Joined Exist Room:" << tmpRoom->second->getRoomId() << endl;
                package->session->setRoomid(tmpRoom->second->getRoomId());
                isExistRoom = true;
                break;
            }
        }
    }
    
    if (!isExistRoom)
    {
        //«ÿ¥Á«œ¥¬ πÊ¿Ã æ¯¿ª∂ß
        int curRoomId = _lastRoomId++;
        Room *newRoom = new Room(this, _io, curRoomId, doSearchGamePacket->mode());
        
        if (newRoom == nullptr)
        {
            return;
        }
        
        if (newRoom->setPlayer(package->session))
        {
            cout << "=>Create Room:" << newRoom->getRoomId() << endl;
            package->session->setRoomid(newRoom->getRoomId());
            newRoom->setRoomState(static_cast<int> (RoomState::WAIT));
            _roomGroup.insert({ curRoomId, newRoom });
            isExistRoom = true;
        }
        else {
            sendFailSearchGame(package->session);
            delete package->packet;
            package->packet = nullptr;
            delete package;
            package = nullptr;
            return;
        }
    }
    delete package->packet;
    package->packet = nullptr;
    delete package;
    package = nullptr;
}

void Server::cancelSearchRoomProcess(Package* package)
{
    cout << endl;
    cout << "==========cancelSearchRoomProcess==========" << endl;
    if (package->session->getRoomid() != -1)
    {
        cout << "roomId:" << package->session->getRoomid() << endl;
        
        std::map<int, realtrick::Room*>::iterator findRoom = _roomGroup.find(package->session->getRoomid());
        
        if (_roomGroup.end() != findRoom) {
            if (findRoom->second->getRoomState() == static_cast<int>(RoomState::WAIT))
            {
                cout << "=>DELETE User:" << package->session->getPid() << " from RoomId:" << findRoom->second->getRoomId() << endl;
                findRoom->second->deletePlayer(package->session->getPid());
                package->session->setRoomid(-1);
                
                if (findRoom->second->isEmptyRoom())
                {
                    deleteRoom(findRoom->second->getRoomId());
                }
                
                sendFailSearchGame(package->session);
                
                delete package->packet;
                package->packet = nullptr;
                delete package;
                package = nullptr;
                return;
                
            }
        }
    }
}

void Server::successLoadGameProcess(Package* package)
{
    cout << endl;
    cout << "==========successLoadGameProcess==========" << endl;
    if (package->session->getRoomid() != -1)
    {
        cout << "=>roomId:" << package->session->getRoomid() << endl;
        
        auto findRoom = _roomGroup.find(package->session->getRoomid());
        
        if (_roomGroup.end() != findRoom) {
            if (findRoom->second->getRoomState() == static_cast<int>(RoomState::READY))
            {
                findRoom->second->setLoadGamePlayer(package->session->getPid());
                delete package->packet;
                package->packet = nullptr;
                delete package;
                package = nullptr;
                return;
            }
        }
    }
}

void Server::successMoveJoystickProcess(Package* package)
{
    cout << endl;
    cout << "==========successMoveJoystickProcess==========" << endl;
    
    
    FlatbufferPacket *packet = package->packet;
    
    auto move = fpacket::GetPacketMoveJoystick(packet->body());
    
    cout << "id:" << move->id() << endl;
    cout << "dir_x:" << move->dir_x() << endl;
    cout << "dir_y:" << move->dir_y() << endl;
    cout << "is_touched:" << move->is_touched() << endl;
    
    if (package->session->getRoomid() != -1)
    {
        cout << "=>roomId:" << package->session->getRoomid() << endl;
        
        auto findRoom = _roomGroup.find(package->session->getRoomid());
        
        if (_roomGroup.end() != findRoom) {
            if (findRoom->second->getRoomState() == static_cast<int>(RoomState::INGAME))
            {
                findRoom->second->moveJoystick((package->packet), package->session->getPid());
                return;
            }
        }
    }
}

#pragma mark - Thread Methods

void Server::timerThread()
{
    while (true)
    {
        while  (false == _timerQueue.empty())
        {
            unsigned int curTickCount = getTickCount();
            if (_timerQueue.top().targetTime > curTickCount) break;
            eventToken et = _timerQueue.top();
            
            if(et.eventType == OP_ROOM_TIMER){
                cout << "timerThread: OP_ROOM_TIMER" <<endl;
                Room* curRoom = getRoom(et.eventId);
                if(curRoom != nullptr && curRoom->getRoomState() == static_cast<int> (RoomState::WAIT)){
                    
                    if( curRoom != nullptr && curRoom->getRoomTimer() == et.targetTime){
                        curRoom->sendPlayerSuccessSearchGame();
                    }
                }
                
            }else if(et.eventType == OP_GAME_TIMER){
                cout << "timerThread: OP_GAME_TIMER" <<endl;
                Room* curRoom = getRoom(et.eventId);
                if(curRoom->getRoomState() == static_cast<int> (RoomState::INGAME)){
                    curRoom->updateGame();
                }
                
            }
            _timerQueue.pop();
        }
    }
}

void Server::acceptThread()
{
    _acceptor.async_accept(
                           _socket,
                           _strand.wrap([this](system::error_code ec) {
        if (!ec) {
            int curSessionID = _lastSessionId++;
            cout << "=>session id:" << curSessionID << endl;
            
            Session* session = new Session(std::move(_socket), this, _io, curSessionID);
            
            if (session == nullptr)
            {
                return;
            }
            
            _sessionGroup.insert(map<int, realtrick::Session*>::value_type(curSessionID, session));
            session->start();
            
            acceptThread();
        }
    }));
}

void Server::workerThread()
{
    while (true){
        Package* package = nullptr;
        
        dequeuePacket(package);
        
        package->packet->decode();
        
        FlatbufferPacket *packet = package->packet;
        cout << "[workerPost] type:" << static_cast <int>  (packet->type()) << endl;
        switch (packet->type())
        {
            case PacketType::LOGIN:
            {
                cout << "[workerPost LOGIN]" << endl;
                loginProcess(package);
                break;
            }
            case PacketType::SIGN_UP:
            {
                cout << "[workerPost SIGN_UP]" << endl;
                signupProcess(package);
                break;
            }
            case PacketType::DO_SEARCH_GAME:
            {
                cout << "[workerPost DO_SEARCH_GAME]" << endl;
                searchRoomProcess(package);
                break;
            }
            case PacketType::CANCEL_SEARCH_GAME:
            {
                cout << "[workerPost CANCEL_SEARCH_GAME]" << endl;
                cancelSearchRoomProcess(package);
                break;
            }
            case PacketType::SUCCESS_LOAD_GAME:
            {
                cout << "[workerPost SUCCESS_LOAD_GAME]" << endl;
                successLoadGameProcess(package);
                break;
            }
            case PacketType::MOVE_JOYSTICK:
            {
                cout << "[workerPost MOVE_JOYSTICK]" << endl;
                successMoveJoystickProcess(package);
                break;
            }
            default:
            {
                cout << "[workerPost Unknowned Packet Detected]" << endl;
                break;
            }
        }
    }
}

#pragma mark - Run Method

void Server::run()
{
	asio::io_service::work work(_io);
	
	for (int i = 0; i < 1; ++i) {
		_ioThreads.push_back(new boost::thread(boost::bind(&boost::asio::io_service::run, &_io)));
	}
    
    _workerThreads.push_back(new boost::thread(boost::bind(&Server::workerThread, this)));
    
    _acceptThread = new boost::thread(boost::bind(&Server::acceptThread, this));
    
    _timerThread = new boost::thread(boost::bind(&Server::timerThread, this));
    
    //enqueueTimer(1, getTickCount()+5000, OP_ROOM_TIMER);
    
    int count = 0;
    while (true) {
		cout << endl;
		cout << "[---------------Game info---------------]" << count++ <<endl;
		cout << "=>Connected User Count:" << _sessionGroup.size() << endl;
		cout << "=>Created Room Count:" << _roomGroup.size() << endl;
        cout << "[---------------------------------------]" << endl;
        sleep(1000);
    }

	_io.stop();
	
	for (auto tp : _ioThreads) {
		tp->join();
		delete tp;
	}
	
	for (auto tp : _workerThreads) {
		tp->join();
		delete tp;
	}
	
	_acceptThread->join();
    _timerThread->join();
	delete _acceptThread;
    
}
