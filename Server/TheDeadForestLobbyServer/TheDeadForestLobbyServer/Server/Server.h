#pragma once

#define _ENABLE_ATOMIC_ALIGNMENT_FIX

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/lockfree/queue.hpp>

#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <map>
#include <queue>

#include "protocol.h"
#include "FlatbufferPacket.h"
#include "Package.h"
#include "DBManager.h"
#include "Session.h"
#include "Room.h"
#include "utils.h"

#define OP_ROOM_TIMER 1
#define OP_GAME_TIMER 2

namespace realtrick
{
    class DBManager;
    
	class Session;
    
	class Room;
    
	class FlatbufferPacket;
    
	class Server
	{

	public:

		Server() :
			_acceptor(_io, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), SERVER_PORT)),
			_socket(_io),
			_strand(_io),
			_packetQueue(512),
            _lastSessionId(0),
            _lastUserId(0),
            _lastRoomId(0)
        {
            _util = Utils();
            _dbManager = new DBManager();
			getLastUserId();
            /*_lastUserId++;
            setLastUserId();*/
        };
        
        struct eventToken {
            int eventId;
            unsigned int targetTime;
            int eventType;
        };
        
        class mycomp {
            bool reverse;
        public:
            mycomp() {}
            bool operator() (const eventToken lhs, const eventToken rhs) const
            {
                return (lhs.targetTime > rhs.targetTime);
            }
        };

        virtual ~Server() = default;
        
        int getSessionId() { return _lastSessionId; }
        
        void setLastUserId();
        
        void getLastUserId();
        
        Session* getSession(int id);
        
        void deleteSession(int id);
        
        Room* getRoom(int roomId);
        
        void deleteRoom(int roomId);
        
        boost::asio::io_service* getIOService();
        
        unsigned int getTickCount();
        
        void enqueueTimer(int id, unsigned int time, int type);
        
        void enqueuePacket(Package *packet);
        
        void dequeuePacket(Package*& package);
        
        bool isQueueEmpty() const { return _packetQueue.empty(); }
        
        void sendSuccessLogin(Session* session, int pid, string nickName);
        
        void sendErrorCode(Session* session, int errorCode);
        
        void sendSuccessSearchGame(Session* session);
        
        void sendFailSearchGame(Session* session);
        
        void sendFisrtPlayerInfos(Session* session, FlatbufferPacket* packet);
        
        void sendGameStart(Session* session);
        
        void sendMoveJoystick(Session* session, FlatbufferPacket *packet);
        
        void loginProcess(Package* package);
        
        void signupProcess(Package* package);
        
        void searchRoomProcess(Package* package);
        
        void cancelSearchRoomProcess(Package* package);
        
        void successLoadGameProcess(Package* package);
        
        void successMoveJoystickProcess(Package* package);

        void timerThread();
        
		void acceptThread();

		void workerThread();
		
        void run();
        
    private:
        
        priority_queue<eventToken, vector<eventToken>, mycomp> _timerQueue;
        
        Utils                                   _util;
        
        DBManager*                              _dbManager;

		boost::asio::io_service					_io;

		boost::asio::io_service::strand			_strand;

		boost::asio::ip::tcp::socket			_socket;

        boost::asio::ip::tcp::acceptor			_acceptor;
        
        std::vector<boost::thread*>				_ioThreads;

        std::vector<boost::thread*>				_workerThreads;
        
        boost::thread*							_acceptThread;
        
        boost::thread*							_timerThread;

		std::map<int, realtrick::Session*>		_sessionGroup;

		boost::lockfree::queue<Package*>		_packetQueue;

        std::map<int, realtrick::Room*>			_roomGroup;
        
        int                                     _lastSessionId;
        
        int                                     _lastUserId;
        
        int                                     _lastRoomId;
        
	};
}