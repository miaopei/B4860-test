/*************************************************************************
	> File Name: hub_client.cpp
	> Author: miaopei
	> Mail: miaopei@baicells.com 
	> Created Time: 2020年02月12日 星期三 11时16分50秒
 ************************************************************************/

#include <iostream>
#include <string.h>

#include "hub.h"

using namespace uv;
using namespace std;

HUB::HUB(uv::EventLoop* loop)
    :TcpClient(loop),
    sockAddr(nullptr)
{
    setConnectStatusCallback(std::bind(&HUB::onConnect, this, std::placeholders::_1));
    setMessageCallback(std::bind(&HUB::RecvMesg, this, std::placeholders::_1, std::placeholders::_2));
}

void HUB::connectToServer(uv::SocketAddr& addr)
{
    sockAddr = std::make_shared<uv::SocketAddr>(addr);
    connect(addr);
}

void HUB::reConnect()
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

void HUB::sendTestMessage()
{
    // packet 封装
#if 1
    std::string data = "key=value&key2=value2";
    uv::PacketIR packetir;
    
    packetir.SetHead(to_string(uv::PacketIR::HUB), 
                     to_string(uv::PacketIR::TO_BBU),
                     to_string(uv::PacketIR::REQUEST),
                     to_string(uv::PacketIR::MSG_CONNECT), 
                     to_string(uv::PacketIR::RRUID_2),
                     to_string(uv::PacketIR::PORT_0),
                     to_string(uv::PacketIR::UPORT_4));

    packetir.PackMessage(data, data.length());
    std::cout << "封装 packet:" << std::endl;
    std::cout << "\tPacket: " << packetir.GetPacket() << std::endl;
    std::cout << "\tHead: " << packetir.GetHead() << std::endl;
    std::cout << "\tSource: " << packetir.GetSource() << std::endl;
	std::cout << "\tDestination: " << packetir.GetDestination() << std::endl;
    std::cout << "\tState: " << packetir.GetState() << std::endl;
    std::cout << "\tMsgID: " << packetir.GetMsgID() << std::endl;
    std::cout << "\tRRUID: " << packetir.GetRRUID() << std::endl;
    std::cout << "\tPort: " << packetir.GetPort() << std::endl;
	std::cout << "\tUPort: " << packetir.GetUPort() << std::endl;
    std::cout << "\tLength: " << packetir.GetLength() << std::endl;
    std::cout << "\tData: " << packetir.GetData() << std::endl;
    std::cout << "\tData Length: " << data.length() << std::endl;

	std::string send_buf = packetir.GetPacket();
	write(send_buf.c_str(), send_buf.length());
#endif
}

void HUB::onConnect(ConnectStatus status)
{
    if(status != ConnectStatus::OnConnectSuccess)
    {
        reConnect();
    } else {
        sendTestMessage();
    }
}

void HUB::newMessage(const char* buf, ssize_t size)
{
    std::cout << "HUB newMessage: " << std::string(buf, size) << std::endl;

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

void HUB::RecvMesg(const char* buf, ssize_t size)
{
    std::cout << "HUB Recv Msg: " << std::string(buf, size) << std::endl;
    //string msg = "ttt";
    //write(msg.c_str(), msg.length());
}

void HUB::SendMesg(const char* buf, ssize_t size)
{
    std::cout << "Client::SendMesg" << std::endl;
    if(uv::GlobalConfig::BufferModeStatus == uv::GlobalConfig::NoBuffer)
    {
        writeInLoop(buf, (unsigned int)size, nullptr);
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

}

void HUB::SendMesgThread()
{
    sendTestMessage();
}

