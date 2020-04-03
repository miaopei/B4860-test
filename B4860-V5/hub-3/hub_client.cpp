/*************************************************************************
	> File Name: hub_client.cpp
	> Author: miaopei
	> Mail: miaopei@baicells.com 
	> Created Time: 2020年02月12日 星期三 11时16分50秒
 ************************************************************************/

#include <iostream>
#include <string.h>

#include "hub_client.h"

using namespace uv;
using namespace std;

Client::Client(uv::EventLoop* loop)
    :TcpClient(loop),
    sockAddr(nullptr)
{
    setConnectStatusCallback(std::bind(&Client::onConnect, this, std::placeholders::_1));
    setMessageCallback(std::bind(&Client::newMessage, this, std::placeholders::_1, std::placeholders::_2));
}

void Client::connectToServer(uv::SocketAddr& addr)
{
    sockAddr = std::make_shared<uv::SocketAddr>(addr);
    connect(addr);
}

void Client::reConnect()
{
    uv::Timer* timer = new uv::Timer(loop_, 500, 0, [this](uv::Timer* ptr)
    {
        connect(*(sockAddr.get()));
        ptr->close([](uv::Timer* ptr)
        {
            delete ptr;    
        });
    });
    timer->start();
}

void Client::sendTestMessage()
{
    char data[1024] = "HUB-3 Client connected!";
    if(uv::GlobalConfig::BufferModeStatus == uv::GlobalConfig::NoBuffer)
    {
        write(data, (int)sizeof(data));
    } else {
        uv::Packet packet;
        packet.pack(data, sizeof(data));
        write(packet.Buffer().c_str(), packet.PacketSize());
    }
}

void Client::onConnect(ConnectStatus status)
{
    if(status != ConnectStatus::OnConnectSuccess)
    {
        reConnect();
    } else {
        sendTestMessage();
    }
}

void Client::newMessage(const char* buf, ssize_t size)
{
    std::cout << "HUB-3 newMessage: " << std::string(buf, size) << std::endl;

#if 0
    if(uv::GlobalConfig::BufferModeStatus == uv::GlobalConfig::NoBuffer)
    {
        write(buf, (unsigned int)size);
    } else {
        auto packetbuf = getCurrentBuf();
        if(nullptr != packetbuf)
        {
            packetbuf->append(buf, static_cast<int>(size));
            uv::Packet packet;
            while(0 == packetbuf->readPacket(packet))
            {
                write(packet.Buffer().c_str(), (unsigned)packet.PacketSize(), nullptr);
            }
        }
    }
#endif
}

