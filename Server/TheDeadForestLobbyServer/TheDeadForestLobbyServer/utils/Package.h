#pragma once

#include"Session.h"
#include"FlatbufferPacket.h"

namespace realtrick
{
	class Session;
	class FlatbufferPacket;
	class Package
	{
	public:
		Session*								session;
		FlatbufferPacket*						packet;

		Package(Session* s, FlatbufferPacket* p)
		{
			session = s;
			packet = p;
		}

		~Package() = default;
	};
}