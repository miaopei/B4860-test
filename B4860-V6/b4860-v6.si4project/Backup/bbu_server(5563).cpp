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
    
	MessageProcess(connection, packetir);
}

void Server::MessageProcess(uv::TcpConnectionPtr& connection, uv::PacketIR& packet)
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

	switch(std::stoi(packet.GetDestination()))
	{
		case uv::PacketIR::TO_BBU:
			std::cout << "[Destiantion: BBU]" << std::endl;
			BBUMessageProcess(connection, packet);
			break;
		case uv::PacketIR::TO_HUB:
			std::cout << "[Destiantion: HUB]" << std::endl;
			HUBMessageProcess(connection, packet);
			break;
		case uv::PacketIR::TO_RRU:
			std::cout << "[Destiantion: HUB]" << std::endl;
			RRUMessageProcess(connection, packet);
			break;
		case uv::PacketIR::TO_OAM:
			std::cout << "[Destiantion: HUB]" << std::endl;
			OAMMessageProcess(connection, packet);
			break;
		default:
			std::cout << "[Error: Destiantion: Error]" << std::endl;
	}
}

void Server::BBUMessageProcess(uv::TcpConnectionPtr& connection, uv::PacketIR& packet)
{
	/* BBU 为消息处理中心，接收到的都是 request 消息*/
	switch(std::stoi(packet.GetMsgID()))
	{
		case uv::PacketIR::MSG_CONNECT:
			std::cout << "[msg_connect]" << std::endl;
			SetConnectionClient(connection, packet);
            break;
		default:
			std::cout << "[Error: MessageID Error]" << std::endl;
	}
}

void Server::HUBMessageProcess(uv::TcpConnectionPtr& connection, uv::PacketIR& packet)
{

}

void Server::RRUMessageProcess(uv::TcpConnectionPtr& connection, uv::PacketIR& packet)
{

}
void Server::OAMMessageProcess(uv::TcpConnectionPtr& connection, uv::PacketIR& packet)
{

}

void Server::SetConnectionClient(uv::TcpConnectionPtr& connection, uv::PacketIR& packet)
{
	ClientInfo cInfo;
	cInfo.s_ip = GetCurrentName(connection);
	cInfo.s_connection = connection;
	cInfo.s_source = packet.GetSource();
	cInfo.s_RRUID = packet.GetRRUID();
	cInfo.s_port = packet.GetPort();
	cInfo.s_uport = packet.GetUPort();

	if(!SetConnectionInfo(connection, cInfo))
	{
		std::cout << "[Error: SetConnectionInfo Error]" << std::endl;
		return;
	}
	
	std::cout << "[SetConnectionInfo Success, Echo Message to " << packet.GetSource() << "]"<< std::endl;

	/* Version Check */
	/* TODO*/

	PackMessageSend(connection, packet);
}

void Server::UnPackData(uv::PacketIR& packet)
{
	/* Json 存储数据 jsoncpp，考虑同时支持 xml 存储数据 */
	
}

void Server::PackMessageSend(uv::TcpConnectionPtr& connection, uv::PacketIR& packet)
{
	std::string data = "CheckResult=0";
    uv::PacketIR packetir;
    
    packetir.SetHead(packet.GetDestination(), 
                     packet.GetSource(),
                     to_string(uv::PacketIR::RESPONSE),
                     packet.GetMsgID(), 
                     packet.GetRRUID(),
                     packet.GetPort(),
                     packet.GetUPort());

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

	SendMessage(connection, send_buf.c_str(), send_buf.length());
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

void Server::SendMessage(shared_ptr<TcpConnection> connection, const char* buf, ssize_t size)
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

void Server::SendAllClientMessage(const char* msg, ssize_t size)
{
    std::map<std::string ,TcpConnectionPtr>  allconnnections;
    getAllConnection(allconnnections);

    for(auto &conn : allconnnections)
    {
        std::cout << "client ip=" << conn.first << std::endl; 
        //conn.second->write(msg, size, nullptr);
        SendMessage(conn.second, msg, size);
    }
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


