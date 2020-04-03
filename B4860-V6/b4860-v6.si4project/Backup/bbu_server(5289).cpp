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
    setMessageCallback(std::bind(&Server::OnMessage, this, placeholders::_1, placeholders::_2, placeholders::_3));
}

void Server::OnMessage(shared_ptr<TcpConnection> connection, const char* buf, ssize_t size)
{
	if(size < HEADLENGTH)
	{
		std::cout << "Message length error." << std::endl;
		return ;
	}

	std::string revb_buf = std::string(buf, size);
    uv::PacketIR packetir;
    packetir.UnPackMessage(revb_buf);
    
    switch(std::stoi(packetir.GetState()))
    {
        case uv::PacketIR::REQUEST:
            std::cout << "[Request]" << std::endl;
            ReqMessageProcess(connection, packetir);
            break;
        case uv::PacketIR::RESPONSE:
            std::cout << "[Response]" << std::endl;
            ResMessageProcess(connection, packetir);
            break;
        default:
			std::cout << "default" << std::endl;
    }

#if 0
    switch(std::stoi(packetir.GetMsgID()))
    {
        case uv::PacketIR::MSG_GET_KEY:
            std::cout << "[msg_get]" << std::endl;
            TestGet();
            break;
		case uv::PacketIR::MSG_SET:
            std::cout << "[msg_set]" << std::endl;
            break;
		case uv::PacketIR::MSG_UPGRADE:
			std::cout << "[msg_upgrade]" << std::endl;
            break;
		case uv::PacketIR::MSG_GET_LOG:
			std::cout << "[msg_get_log]" << std::endl;
            break;
		case uv::PacketIR::MSG_GET_NETWORK_TOPOLOGY:
			std::cout << "[msg_get_network_topology]" << std::endl;
            NetworkTopology();
            break;
		case uv::PacketIR::MSG_CONNECT:
			std::cout << "[msg_connect]" << std::endl;
			SetConnectionClient(connection, packetir);
            break;
		case uv::PacketIR::MSG_DELAY_MEASUREMENT:
			std::cout << "[msg_delay_measurement]" << std::endl;
            break;
		default:
			std::cout << "default" << std::endl;
    }
#endif

#if 0
    std::string revb_buf = std::string(buf, size);
    uv::PacketIR packetir;
    packetir.UnPackMessage(revb_buf);
    std::cout << "解析 packet:" << std::endl;
    std::cout << "\tGetPacket: " << packetir.GetPacket() << std::endl;
    std::cout << "\tGetHead: " << packetir.GetHead() << std::endl;
    std::cout << "\tGetType: " << packetir.GetType() << std::endl;
    std::cout << "\tGetMsgID: " << packetir.GetMsgID() << std::endl;
    std::cout << "\tGetState: " << packetir.GetState() << std::endl;
    std::cout << "\tGetRRUID: " << packetir.GetRRUID() << std::endl;
    std::cout << "\tGetPort: " << packetir.GetPort() << std::endl;
    std::cout << "\tGetData: " << packetir.GetData() << std::endl;

    WriteMessage(connection, buf, size);
#endif
}

void Server::writeCallback(uv::WriteInfo& info)
{
    uv::LogWriter::Instance()->debug("Server::writeCallback");
    if(0 != info.status)
    {
        std::cout << "Write error ：" << EventLoop::GetErrorMessage(info.status) << std::endl;
    }
    delete[] info.buf;
}

void Server::WriteMessage(shared_ptr<TcpConnection> connection, const char* buf, ssize_t size)
{
    if(uv::GlobalConfig::BufferModeStatus == uv::GlobalConfig::NoBuffer)
    {
        connection->write(buf, size, nullptr);
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
}

void Server::SendMessage(const char* msg, ssize_t size)
{
    std::map<std::string ,TcpConnectionPtr>  allconnnections;
    getAllConnection(allconnnections);

    for(auto &conn : allconnnections)
    {
        std::cout << "client ip=" << conn.first << std::endl; 
        //conn.second->write(msg, size, nullptr);
        WriteMessage(conn.second, msg, size);
    }
}

bool Server::SetConnectionClient(uv::TcpConnectionPtr connection, uv::PacketIR packetir)
{
	ClientInfo cInfo;
	cInfo.s_ip = GetCurrentName(connection);
	cInfo.s_connection = connection;
	cInfo.s_source = packetir.GetSource();
	cInfo.s_RRUID = packetir.GetRRUID();
	cInfo.s_port = packetir.GetPort();
	cInfo.s_uport = packetir.GetUPort();

	if(SetConnectionInfo(connection, cInfo))
	{
        return true;
	}
	std::cout << "SetConnectionInfo error" << std::endl;
	return false;
}

void Server::NetworkTopology()
{
    std::map<std::string, ClientInfo> netTopology;
    GetNetworkTopology(netTopology);

    for(auto &it : netTopology)
    { 
        std::cout << "netTopology: " 
            << it.first << " - > " 
            << it.second.s_ip << " "
            << it.second.s_connection << " "
            << it.second.s_source << " "
            << it.second.s_RRUID << " " 
            << it.second.s_port << " "
            << it.second.s_uport << std::endl;
    }
}

void Server::TestGet()
{
    std::vector<TcpConnectionPtr> hubsConnection;
    GetHUBsConnection(hubsConnection);
    for(auto &it : hubsConnection)
    {
        std::cout << "hubsConnection: " << it << std::endl;
    }

    std::vector<TcpConnectionPtr> rrusConnection;
    GetRRUsConnection(rrusConnection);
    for(auto &it : rrusConnection)
    {
        std::cout << "rrusConnection: " << it << std::endl;
    }
}

void Server::ReqMessageProcess(uv::TcpConnectionPtr connection, uv::PacketIR& packet)
{
    std::cout << "解析 packet:" << std::endl;
    std::cout << "\tPacket: " << packet.GetPacket() << std::endl;
    std::cout << "\tHead: " << packet.GetHead() << std::endl;
    std::cout << "\tSource: " << packet.GetSource() << std::endl;
	std::cout << "\tDestination: " << packet.GetDestination() << std::endl;
    std::cout << "\tState: " << packet.GetState() << std::endl;
    std::cout << "\tMsgID: " << packet.GetMsgID() << std::endl;
    std::cout << "\tRRUID: " << packet.GetRRUID() << std::endl;
    std::cout << "\tPort: " << packet.GetPort() << std::endl;
	std::cout << "\tUPort: " << packet.GetUPort() << std::endl;
    std::cout << "\tLength: " << packet.GetLength() << std::endl;
    std::cout << "\tData: " << packet.GetData() << std::endl;

}

void Server::ResMessageProcess(uv::TcpConnectionPtr connection, uv::PacketIR& packet)
{
    std::cout << "解析 packet:" << std::endl;
    std::cout << "\tPacket: " << packet.GetPacket() << std::endl;
    std::cout << "\tHead: " << packet.GetHead() << std::endl;
    std::cout << "\tSource: " << packet.GetSource() << std::endl;
	std::cout << "\tDestination: " << packet.GetDestination() << std::endl;
    std::cout << "\tState: " << packet.GetState() << std::endl;
    std::cout << "\tMsgID: " << packet.GetMsgID() << std::endl;
    std::cout << "\tRRUID: " << packet.GetRRUID() << std::endl;
    std::cout << "\tPort: " << packet.GetPort() << std::endl;
	std::cout << "\tUPort: " << packet.GetUPort() << std::endl;
    std::cout << "\tLength: " << packet.GetLength() << std::endl;
    std::cout << "\tData: " << packet.GetData() << std::endl;

}

