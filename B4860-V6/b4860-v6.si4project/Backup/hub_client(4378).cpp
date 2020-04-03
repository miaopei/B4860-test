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
    setMessageCallback(std::bind(&Client::RecvMessage, this, std::placeholders::_1, std::placeholders::_2));
    SetRHUBInfo();
}

void Client::connectToServer(uv::SocketAddr& addr)
{
    sockAddr = std::make_shared<uv::SocketAddr>(addr);
    connect(addr);
}

void Client::onConnect(ConnectStatus status)
{
    if(status != ConnectStatus::OnConnectSuccess)
    {
        reConnect();
    } else {
        SendConnectMessage();
    }
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

void Client::SendConnectMessage()
{
    std::string data = "Version=1.0";
    uv::PacketIR packetir;
    
    packetir.SetHead(m_source, 
                     to_string(uv::PacketIR::TO_BBU),
                     to_string(uv::PacketIR::REQUEST),
                     to_string(uv::PacketIR::MSG_CONNECT), 
                     m_rruid,
                     m_port,
                     m_uport);

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
}

void Client::SetRHUBInfo()
{
    int mpi_fd = gpmc_mpi_open(GPMC_MPI_DEV);
    /* 获取 rhub 的 port id 信息 */
    uint16_t rhub_port = get_rhup_port_id(mpi_fd, UP);

    std::cout << "rhub_port = " << rhub_port << std::endl;
    std::cout << "port = " << ((rhub_port >> 8) & 0xf) << std::endl;
    std::cout << "rruid = " << ((rhub_port >> 4) & 0xf) << std::endl;
    std::cout << "uport = " << (rhub_port & 0xf) << std::endl;

    m_source = to_string(uv::PacketIR::HUB);
    m_port = to_string(((rhub_port >> 8) & 0xf));
    m_rruid = to_string(((rhub_port >> 4) & 0xf));
    m_uport = to_string((rhub_port & 0xf));

    gpmc_mpi_close(mpi_fd);
}

void Client::GetRHUBDelayInfo()
{
    int mpi_fd = gpmc_mpi_open(GPMC_MPI_DEV);
    /* 获取 rhub 的处理时延  */
    struct rhup_data_delay* rhup_delay = (struct rhup_data_delay*)malloc(sizeof(struct rhup_data_delay));
    get_rhup_delay(mpi_fd, UP, rhup_delay);

#if 1
    std::cout << "delay1 = " << rhup_delay->delay1 
              << "\ndelay2 = " << rhup_delay->delay2
              << "\ndelay3 = " << rhup_delay->delay3
              << "\ndelay4 = " << rhup_delay->delay4
              << "\ndelay5 = " << rhup_delay->delay5
              << "\ndelay6 = " << rhup_delay->delay6
              << "\ndelay7 = " << rhup_delay->delay7
              << "\ndelay8 = " << rhup_delay->delay8
              << "\ndelay9 = " << rhup_delay->delay9
              << std::endl;
#endif

    /* 获取 rhub T14 测量时延信息 */
    struct rhup_t14_delay* t14_delay = (struct rhup_t14_delay*)malloc(sizeof(struct rhup_t14_delay));
    get_rhup_t14_delay(mpi_fd, t14_delay);

#if 1
    std::cout << "rhub级联口的T14 = " << t14_delay->delay1
              << "\nrhub下联 0 号口的T14 = " << t14_delay->delay2
              << "\nrhub下联 1 号口的T14 = " << t14_delay->delay3
              << "\nrhub下联 2 号口的T14 = " << t14_delay->delay4
              << "\nrhub下联 3 号口的T14 = " << t14_delay->delay5
              << "\nrhub下联 4 号口的T14 = " << t14_delay->delay6
              << "\nrhub下联 5 号口的T14 = " << t14_delay->delay7
              << "\nrhub下联 6 号口的T14 = " << t14_delay->delay8
              << "\nrhub下联 7 号口的T14 = " << t14_delay->delay9
              << std::endl;

    std::cout << "Toffset = " << TOFFSET << std::endl;
#endif

    free(rhup_delay);
    free(t14_delay);
    gpmc_mpi_close(mpi_fd);

#if 0
    rhub_port = 16
    port = 0
    rruid = 1
    uport = 0
    delay1 = 1175
    delay2 = 24
    delay3 = 24
    delay4 = 24
    delay5 = 24
    delay6 = 24
    delay7 = 24
    delay8 = 24
    delay9 = 1
    rhub级联口的T14 = 1097088
    rhub下联 0 号口的T14 = 153
    rhub下联 1 号口的T14 = 1096878
    rhub下联 2 号口的T14 = 1096878
    rhub下联 3 号口的T14 = 1096878
    rhub下联 4 号口的T14 = 1096878
    rhub下联 5 号口的T14 = 1096878
    rhub下联 6 号口的T14 = 1096878
    rhub下联 7 号口的T14 = 1096878
#endif
}

void Client::RecvMessage(const char* buf, ssize_t size)
{
    std::cout << "HUB Recv Msg: " << std::string(buf, size) << std::endl;

    /* 接收到的数据解析 */
}

void Client::SendMessage(const char* buf, ssize_t size)
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

