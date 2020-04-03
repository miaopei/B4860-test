/*************************************************************************
	> File Name: hub.cpp
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
    setMessageCallback(std::bind(&HUB::RecvMessage, this, std::placeholders::_1, std::placeholders::_2));
    //setMessageCallback(std::bind(&HUB::SendMessage, this, std::placeholders::_1, std::placeholders::_2));
    SetRHUBInfo();
}

void HUB::connectToServer(uv::SocketAddr& addr)
{
    sockAddr = std::make_shared<uv::SocketAddr>(addr);
    connect(addr);
}

void HUB::onConnect(ConnectStatus status)
{
    if(status != ConnectStatus::OnConnectSuccess)
    {
        reConnect();
    } else {
        SendConnectMessage();
    }
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

void HUB::SendConnectMessage()
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

    /* 打印数据封装信息 */
    packetir.EchoPackMessage();

    std::string send_buf = packetir.GetPacket();
	write(send_buf.c_str(), send_buf.length());
}

void HUB::SetRHUBInfo()
{
#if 0
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
#endif
    m_source = "0";
    m_port = "2";
    m_rruid = "3";
    m_uport = "4";
}

void HUB::SendRHUBDelayInfo()
{
#if 0
    int mpi_fd = gpmc_mpi_open(GPMC_MPI_DEV);
    
    struct delay_measurement_info* dm_info = (struct delay_measurement_info*)malloc(sizeof(struct delay_measurement_info));

    /* 获取 rhub 的处理时延  */
    //struct rhup_data_delay* rhup_delay = (struct rhup_data_delay*)malloc(sizeof(struct rhup_data_delay));
    get_rhup_delay(mpi_fd, UP, dm_info->rhup_delay);

#if 0
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
    //struct rhup_t14_delay* t14_delay = (struct rhup_t14_delay*)malloc(sizeof(struct rhup_t14_delay));
    get_rhup_t14_delay(mpi_fd, dm_info->t14_delay);

#if 0
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

    /* 封装时延测量数据并发送到 BBU */ 

#if 1
    std::cout << "delay1 = " << dm_info->rhup_delay->delay1 
              << "\ndelay2 = " << dm_info->rhup_delay->delay2
              << "\ndelay3 = " << dm_info->rhup_delay->delay3
              << "\ndelay4 = " << dm_info->rhup_delay->delay4
              << "\ndelay5 = " << dm_info->rhup_delay->delay5
              << "\ndelay6 = " << dm_info->rhup_delay->delay6
              << "\ndelay7 = " << dm_info->rhup_delay->delay7
              << "\ndelay8 = " << dm_info->rhup_delay->delay8
              << "\ndelay9 = " << dm_info->rhup_delay->delay9
              << std::endl;
#endif

#if 1
    std::cout << "rhub级联口的T14 = " << dm_info->t14_delay->delay1
              << "\nrhub下联 0 号口的T14 = " << dm_info->t14_delay->delay2
              << "\nrhub下联 1 号口的T14 = " << dm_info->t14_delay->delay3
              << "\nrhub下联 2 号口的T14 = " << dm_info->t14_delay->delay4
              << "\nrhub下联 3 号口的T14 = " << dm_info->t14_delay->delay5
              << "\nrhub下联 4 号口的T14 = " << dm_info->t14_delay->delay6
              << "\nrhub下联 5 号口的T14 = " << dm_info->t14_delay->delay7
              << "\nrhub下联 6 号口的T14 = " << dm_info->t14_delay->delay8
              << "\nrhub下联 7 号口的T14 = " << dm_info->t14_delay->delay9
              << std::endl;

    std::cout << "Toffset = " << TOFFSET << std::endl;
#endif

    uv::PacketIR packet;

    packet.SetHead(to_string(uv::PacketIR::HUB),
                   to_string(uv::PacketIR::TO_BBU),
                   to_string(uv::PacketIR::RESPONSE),
                   to_string(uv::PacketIR::MSG_DELAY_MEASUREMENT),
                   m_rruid, m_port, m_uport);

    

    //free(rhup_delay);
    //free(t14_delay);
    free(dm_info);
    gpmc_mpi_close(mpi_fd);
#endif

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

void HUB::RecvMessage(const char* buf, ssize_t size)
{
    if(size < HEADLENGTH)
    {
        std::cout << "Message Length error." << std::endl;
        return;
    }

    /* 接收到的数据解析 */
    std::string revb_buf = std::string(buf, size);
    uv::PacketIR packet;
    packet.UnPackMessage(revb_buf);

    /* 打印解包信息 */
    packet.EchoUnPackMessage();

    switch(std::stoi(packet.GetMsgID()))
    {
        case uv::PacketIR::MSG_CONNECT:
            std::cout << "[RCV:msg_connect]" << std::endl;
            ConnectResultProcess(packet);
            break;
        default:
            std::cout << "[Error: MessageID Error]" << std::endl;
    }
}

void HUB::SendMessage(const char* buf, ssize_t size)
{
    std::cout << "Client::SendMesg" << std::endl;
    if(uv::GlobalConfig::BufferModeStatus == uv::GlobalConfig::NoBuffer)
    {
        //writeInLoop(buf, (unsigned int)size, nullptr);
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

void HUB::ConnectResultProcess(uv::PacketIR& packet)
{
    //SendRHUBDelayInfo();
    TestProcess(packet);
}

void HUB::TestProcess(uv::PacketIR& packet)
{
    uv::PacketIR packetir;

    packetir.SetHead(to_string(uv::PacketIR::HUB),
                   to_string(uv::PacketIR::TO_BBU),
                   to_string(uv::PacketIR::RESPONSE),
                   to_string(uv::PacketIR::MSG_DELAY_MEASUREMENT),
                   m_rruid, m_port, m_uport);

    std::string head = packetir.GetHead();
#if 1 
    struct delay_measurement_info dm_info;
    memset(&dm_info, 0, sizeof(struct delay_measurement_info));
    //(struct delay_measurement_info*)malloc(sizeof(struct delay_measurement_info));
    memcpy(dm_info.head, head.c_str(), sizeof(dm_info.head)); 

    struct rhub_data_delay* rhub_up = (struct rhub_data_delay*)malloc(sizeof(struct rhub_data_delay));
    memset(rhub_up, 0, sizeof(struct rhub_data_delay));
    TestGetRhubDelay(0, rhub_up);
    dm_info.rhub_delay_up = rhub_up;

    struct rhub_data_delay* rhub_down = (struct rhub_data_delay*)malloc(sizeof(struct rhub_data_delay));
    memset(rhub_down, 0, sizeof(struct rhub_data_delay));
    TestGetRhubDelay(1, rhub_down);
    dm_info.rhub_delay_down = rhub_down;

    struct rhub_t14_delay* t14_delay = (struct rhub_t14_delay*)malloc(sizeof(struct rhub_t14_delay));
    TestGetRhubT14Delay(t14_delay);
    dm_info.rhub_t14 = t14_delay;
    
    char buf[sizeof(struct rhub_data_delay)+1];
    memcpy(buf, &rhub_up, sizeof(struct rhub_data_delay));

    std::cout << "buf=" << buf << std::endl;
    //SendMessage(buf, sizeof(buf));

    //free(dm_info);
    //free(rhub_up);
    free(rhub_down);
    free(t14_delay);
#endif
}

void HUB::TestGetRhubDelay(uint8_t dir, struct rhub_data_delay* rhub_delay)
{
    if(dir == 0)
    {
        rhub_delay->delay1 = 24;
        rhub_delay->delay2 = 25;
    } else if (dir == 1) {
        rhub_delay->delay1 = 26;
        rhub_delay->delay2 = 27;
    }
}

void HUB::TestGetRhubT14Delay(struct rhub_t14_delay* t14_delay)
{
    t14_delay->delay1 = 11456;
    t14_delay->delay2 = 11457;
}



