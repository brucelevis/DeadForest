
#include "Session.h"

using namespace realtrick;
using namespace std;

void Session::start() {
	doReadHeader();
}

void Session::doReadHeader()
{
	boost::asio::async_read(
		_socket, 
		boost::asio::buffer(_recvBuf.data(), FlatbufferPacket::HEADER_LENGTH),
		_strand.wrap([this](boost::system::error_code ec, size_t length)
	{
		if (!ec && _recvBuf.decode())
		{
			doReadBody();
		}
		else
		{
			_socket.close(ec);
		}
	}));
}

void Session::doRead()
{
    _socket.async_read_some(boost::asio::buffer(data_, max_length),
                            [this](boost::system::error_code ec, std::size_t length)
                            {
                                if (!ec)
                                {
                                    int data_to_process = static_cast<int>(length);
                                    unsigned char * buf = data_;
                                    while (0 < data_to_process) {
                                        if (0 == curr_packet_size_) {
                                            curr_packet_size_ = buf[0];
                                            if (buf[0] > 200) {
                                                cout << "Invalid Packet Size [ << buf[0] << ] Terminating Server!\n";
                                                exit(-1);
                                            }
                                        }
                                        int need_to_build = curr_packet_size_ - prev_data_size_;
                                        if (need_to_build <= data_to_process) {
                                            
                                            memcpy(packet_ + prev_data_size_, buf, need_to_build);
                                            //Process_Packet(packet_, my_id_);
                                            curr_packet_size_ = 0;
                                            prev_data_size_ = 0;
                                            data_to_process -= need_to_build;
                                            buf += need_to_build;
                                        }
                                        else {
                                            
                                            memcpy(packet_ + prev_data_size_, buf, data_to_process);
                                            prev_data_size_ += data_to_process;
                                            data_to_process = 0;
                                            buf += data_to_process;
                                        }
                                    }
                                    doRead();
                                }
                                else
                                {
                                    _socket.close(ec);
                                }
                            });
}

void Session::doReadBody()
{
	boost::asio::async_read(
		_socket,
		boost::asio::buffer(_recvBuf.body(), _recvBuf.bodyLength()),
		_strand.wrap([this](boost::system::error_code ec, size_t length)
	{
		if (!ec)
		{
			cout << "=>arrived packet: " << static_cast <int> (_recvBuf.type()) << endl;

			switch (_recvBuf.type())
			{

			case PacketType::LOGIN:
			{
				cout << "[LOGIN]"<< endl;

				auto obj = fpacket::GetPacketLogin(_recvBuf.body());

				flatbuffers::FlatBufferBuilder builder;
				fpacket::PacketLoginBuilder packetLoginBuilder(builder);
				packetLoginBuilder.add_pid(obj->pid());
				auto temp = packetLoginBuilder.Finish();
				builder.Finish(temp);

				uint8_t* body = builder.GetBufferPointer();
				uint32_t size = builder.GetSize();

				FlatbufferPacket* packet = new FlatbufferPacket();
				packet->encode(body, size, PacketType::LOGIN);

				_server->enqueuePacket(new Package(this, packet));

				break;
			}
			case PacketType::SIGN_UP:
			{
				cout << "[SIGN_UP]" << endl;

				auto obj = fpacket::GetPacketSignup(_recvBuf.body());

				flatbuffers::FlatBufferBuilder builder;
				fpacket::PacketSignupBuilder packetSignupBuilder(builder);
				packetSignupBuilder.add_id(builder.CreateString(obj->id()));
				packetSignupBuilder.add_pid(obj->pid());
				auto temp = packetSignupBuilder.Finish();
				builder.Finish(temp);

				uint8_t* body = builder.GetBufferPointer();
				uint32_t size = builder.GetSize();

				FlatbufferPacket* packet = new FlatbufferPacket();
				packet->encode(body, size, PacketType::SIGN_UP);

				_server->enqueuePacket(new Package(this, packet));

				break;
			}
			case PacketType::DO_SEARCH_GAME:
			{
				cout << "[DO_SEARCH_GAME]" << endl;
				auto obj = fpacket::GetPacketDoSearchGame(_recvBuf.body());

				flatbuffers::FlatBufferBuilder builder;
				fpacket::PacketDoSearchGameBuilder packetDoSearchGameBuilder(builder);
				packetDoSearchGameBuilder.add_mode(obj->mode());
				auto temp = packetDoSearchGameBuilder.Finish();
				builder.Finish(temp);

				uint8_t* body = builder.GetBufferPointer();
				uint32_t size = builder.GetSize();

				FlatbufferPacket* packet = new FlatbufferPacket();
				packet->encode(body, size, PacketType::DO_SEARCH_GAME);

				_server->enqueuePacket(new Package(this, packet));

				break;
			}
			case PacketType::CANCEL_SEARCH_GAME:
			{
				cout << "[CANCEL_SEARCH_GAME]" << endl;
				FlatbufferPacket* packet = new FlatbufferPacket();
				packet->encode(nullptr, 0, PacketType::CANCEL_SEARCH_GAME);

				_server->enqueuePacket(new Package(this, packet));
				break;
			}
			case PacketType::SUCCESS_LOAD_GAME:
			{
				cout << "[SUCCESS_LOAD_GAME]" << endl;
				FlatbufferPacket* packet = new FlatbufferPacket();
				packet->encode(nullptr, 0, PacketType::SUCCESS_LOAD_GAME);

				_server->enqueuePacket(new Package(this, packet));
				break;
			}
			case PacketType::MOVE_JOYSTICK:
			{
				cout << "[MOVE_JOYSTICK]" << endl;

				auto obj = fpacket::GetPacketMoveJoystick(_recvBuf.body());

				flatbuffers::FlatBufferBuilder builder;
				fpacket::PacketMoveJoystickBuilder packetMoveJoystickBuilder(builder);
				packetMoveJoystickBuilder.add_dir_x(obj->dir_x());
				packetMoveJoystickBuilder.add_dir_y(obj->dir_y());
				packetMoveJoystickBuilder.add_id(obj->id());
				packetMoveJoystickBuilder.add_is_touched(obj->is_touched());
				auto temp = packetMoveJoystickBuilder.Finish();
				builder.Finish(temp);

				uint8_t* body = builder.GetBufferPointer();
				uint32_t size = builder.GetSize();

				FlatbufferPacket* packet = new FlatbufferPacket();
				packet->encode(body, size, PacketType::MOVE_JOYSTICK);

				_server->enqueuePacket(new Package(this, packet));
				break;
			}
			default:
			{
				cout << "[Unknowned Packet Detected]type:" << static_cast <int> (_recvBuf.type()) << endl;
				break;
			}
			}
			doReadHeader();
		}
		else
		{
			_socket.close(ec);
		}
	}));
}

void Session::doWrite()
{
	boost::asio::async_write(
		_socket,
		boost::asio::buffer(_writeBuf.front()->data(), _writeBuf.front()->length()),
		_strand.wrap(
		[this](boost::system::error_code ec, std::size_t length)
	{
		printf("=>send packet: %s\n", _writeBuf.front()->data());

		if (!ec)
		{
			FlatbufferPacket* packet = _writeBuf.front();
			delete packet;
			packet = nullptr;

			_writeBuf.pop_front();
			if (!_writeBuf.empty()) doWrite();
		}
		else
		{
			_socket.close(ec);
		}
	}));
}
	