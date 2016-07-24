#pragma once


#include <boost/asio.hpp>

#include <iostream>
#include <memory>
#include <utility>
#include <deque>
#include <vector>

#include "protocol.h"
#include "Server.h"
#include "FlatbufferPacket.h"
#include "GeneratedFlatbufferPackets.hpp"
#include "Package.h"

namespace realtrick
{
	class Server;

	class FlatbufferPacket;

	class Session : public std::enable_shared_from_this<Session>
	{
	public:

		Session(boost::asio::ip::tcp::socket s, Server *server, boost::asio::io_service &io, int sessionId) :
			_socket(std::move(s)),
			_server(server),
			_strand(io),
            _sessionId(sessionId),
            _roomId(-1)
		{};

		virtual ~Session() = default;

		void start();

		void write(FlatbufferPacket* packet)
		{
			bool isWriteProgress = !_writeBuf.empty();
			_writeBuf.push_back(packet);

			if (!isWriteProgress) doWrite();
        }
        
        int getSessionId() { return _sessionId; }
        
        void setPid(int pid) { _pid = pid; }
        
        int getPid() { return _pid; }
        
        void setRoomid(int roomId) { _roomId = roomId; }
        
        int getRoomid() { return _roomId; }
        
        void setNickname(std::string nickName) { _nickName = nickName; }
        
        std::string getNickname() { return _nickName; }

	private:

        void doReadHeader();
        
        void doRead();

		void doReadBody();

		void doWrite();


	private:

		Server*								_server;

		boost::asio::io_service				_io;

		boost::asio::io_service::strand		_strand;

		boost::asio::ip::tcp::socket		_socket;

		FlatbufferPacket					_recvBuf;

		std::deque<FlatbufferPacket*>		_writeBuf;
        
        int                                 _sessionId;
        
        int									_pid;
        
        int									_roomId;
        
        std::string							_nickName;
        
        enum { max_length = 1024 };
        
        unsigned char data_[max_length];
        
        unsigned char packet_[max_length];
        
        int curr_packet_size_;
        
        int prev_data_size_;

	};
}