/*************************************************************************
	> File Name: bbu_server.cpp
	> Author: miaopei
	> Mail: miaopei@baicells.com 
	> Created Time: 2020年02月12日 星期三 11时15分05秒
 ************************************************************************/

#include <cstring>
#include <iostream>

#include "bbu_server.h"

using namespace uv;
using namespace std;

Server::Server(EventLoop* loop)
    :TcpServer(loop)
{
    setMessageCallback(std::bind(&Server::newMessage, this, placeholders::_1, placeholders::_2, placeholders::_3));
}

void Server::newMessage(shared_ptr<TcpConnection> connection, const char* buf, ssize_t size)
{
    std::cout << "BBU newMessage: " << std::string(buf, size) << std::endl;

#if 1
    if(uv::GlobalConfig::BufferModeStatus == uv::GlobalConfig::NoBuffer)
    {
        string msg = "666";
        connection->write(msg.c_str(), msg.length(), nullptr);
        //connection->write(buf, size, nullptr);
    } else {
        Packet packet;
        auto packetbuf = connection->getPacketBuffer();
        if (nullptr != packetbuf)
        {
            packetbuf->append(buf, static_cast<int>(size));

            while (0 == packetbuf->readPacket(packet))
            {
                std::cout << "reserver data " << packet.DataSize() << ":" << packet.getData() << std::endl;
                connection->write(packet.Buffer().c_str(), packet.PacketSize(), nullptr);
            }
        }
    }
#endif
}

