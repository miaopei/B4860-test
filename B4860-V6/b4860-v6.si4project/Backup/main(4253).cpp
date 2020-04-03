/*************************************************************************
	> File Name: main.cpp
	> Author: miaopei
	> Mail: miaopei@baicells.com 
	> Created Time: 2020年02月12日 星期三 11时16分50秒
 ************************************************************************/

#include <iostream>
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <cstdio>
#include <cstring>
#include <vector>

#include "hub_client.h"

using namespace uv;
using namespace std;

std::string serverIP;

string& ClearHeadTailSpace(string &str)
{
    if(str.empty())
    {
        return str;
    }

    str.erase(0, str.find_first_not_of(" "));
    str.erase(str.find_last_not_of(" "));
    return str;
}

void sendMessageThread(Client client)
{
#if 0
    thread sendMessage([&client](){
        std::string data = "key=value&key2=value2";
        //client.sendTestMessage();
	    uv::PacketIR packetir;
	    
	    packetir.SetHead(uv::PacketIR::HUB, 
	                     uv::PacketIR::MSG_GET_NETWORK_TOPOLOGY, 
	                     uv::PacketIR::REQUEST,
	                     uv::PacketIR::RRUID_2,
	                     uv::PacketIR::PORT_6);

        std::cout << "sendMessageThread" << std::endl;
	    packetir.PackMessage(data, data.length());
	    std::cout << "封装 packet:" << std::endl;
	    std::cout << "\tGetPacket: " << packetir.GetPacket() << std::endl;
	    std::cout << "\tGetHead: " << packetir.GetHead() << std::endl;
	    std::cout << "\tGetType: " << packetir.GetType() << std::endl;
	    std::cout << "\tGetMsgID: " << packetir.GetMsgID() << std::endl;
	    std::cout << "\tGetState: " << packetir.GetState() << std::endl;
	    std::cout << "\tGetRRUID: " << packetir.GetRRUID() << std::endl;
	    std::cout << "\tGetPort: " << packetir.GetPort() << std::endl;
	    std::cout << "\tGetLength: " << packetir.GetLength() << std::endl;
	    std::cout << "\tGetData: " << packetir.GetData() << std::endl;
	    std::cout << "\tData Length: " << data.length() << std::endl;

		std::string send_buf = packetir.GetPacket();
		//char* data = new char[send_buf.length()]{};	
		//strcpy(data,send_buf.c_str());
		std::cout << "send_buf=" << send_buf << std::endl;
		//std::cout << "data=" << data << std::endl;
		client.write(send_buf.c_str(), send_buf.length());
    });
    sendMessage.detach();
    this_thread::sleep_for(chrono::milliseconds(200));
#endif
#if 0
    client.setMessageCallback(
        [&client](const char* buf, ssize_t size)
    {
        std::cout << std::string(buf, size) << std::endl;

        std::string data = "key=value&key2=value2";
        //client.sendTestMessage();
	    uv::PacketIR packetir;
	    
	    packetir.SetHead(uv::PacketIR::HUB, 
	                     uv::PacketIR::MSG_GET_NETWORK_TOPOLOGY, 
	                     uv::PacketIR::REQUEST,
	                     uv::PacketIR::RRUID_2,
	                     uv::PacketIR::PORT_6);

        std::cout << "sendMessageThread" << std::endl;
	    packetir.PackMessage(data, data.length());
	    std::cout << "封装 packet:" << std::endl;
	    std::cout << "\tGetPacket: " << packetir.GetPacket() << std::endl;
	    std::cout << "\tGetHead: " << packetir.GetHead() << std::endl;
	    std::cout << "\tGetType: " << packetir.GetType() << std::endl;
	    std::cout << "\tGetMsgID: " << packetir.GetMsgID() << std::endl;
	    std::cout << "\tGetState: " << packetir.GetState() << std::endl;
	    std::cout << "\tGetRRUID: " << packetir.GetRRUID() << std::endl;
	    std::cout << "\tGetPort: " << packetir.GetPort() << std::endl;
	    std::cout << "\tGetLength: " << packetir.GetLength() << std::endl;
	    std::cout << "\tGetData: " << packetir.GetData() << std::endl;
	    std::cout << "\tData Length: " << data.length() << std::endl;

		std::string send_buf = packetir.GetPacket();
		//char* data = new char[send_buf.length()]{};	
		//strcpy(data,send_buf.c_str());
		std::cout << "send_buf=" << send_buf << std::endl;
		//std::cout << "data=" << data << std::endl;
		client.write(send_buf.c_str(), send_buf.length());

    });
#endif    
}

# if 0
void sendMessageThread_2(uv::SocketAddr& addr)
{
    thread sendMessage([&addr](){
        uv::EventLoop loop;
        Client client(&loop);
        client.connectToServer(addr);

        std::string data = "key=value&key2=value2";
        //client.sendTestMessage();
	    uv::PacketIR packetir;
	    
	    packetir.SetHead(uv::PacketIR::HUB, 
	                     uv::PacketIR::TO_BBU,
	                     uv::PacketIR::REQUEST,
	                     uv::PacketIR::MSG_GET_NETWORK_TOPOLOGY, 
	                     uv::PacketIR::RRUID_2,
	                     uv::PacketIR::PORT_6,
	                     uv::PacketIR::UPORT_4);

        std::cout << "sendMessageThread" << std::endl;
	    packetir.PackMessage(data, data.length());
	    std::cout << "封装 packet:" << std::endl;
	    std::cout << "\tPacket: " << packetir.GetPacket() << std::endl;
	    std::cout << "\tHead: " << packetir.GetHead() << std::endl;
	    std::cout << "\tSource: " << packetir.GetSource() << std::endl;
	    std::cout << "\tState: " << packetir.GetState() << std::endl;
	    std::cout << "\tMsgID: " << packetir.GetMsgID() << std::endl;
	    std::cout << "\tRRUID: " << packetir.GetRRUID() << std::endl;
	    std::cout << "\tPort: " << packetir.GetPort() << std::endl;
	    std::cout << "\tLength: " << packetir.GetLength() << std::endl;
	    std::cout << "\tData: " << packetir.GetData() << std::endl;
	    std::cout << "\tData Length: " << data.length() << std::endl;

		std::string send_buf = packetir.GetPacket();
		//char* data = new char[send_buf.length()]{};	
		//strcpy(data,send_buf.c_str());
		std::cout << "send_buf=" << send_buf << std::endl;
		//std::cout << "data=" << data << std::endl;
		client.write(send_buf.c_str(), send_buf.length());
        loop.run();
    });
    sendMessage.detach();
    this_thread::sleep_for(chrono::milliseconds(200));
}
#endif

int main(int argc, char* argv[])
{
    EventLoop* loop = new EventLoop();

    //uv::GlobalConfig::BufferModeStatus = uv::GlobalConfig::CycleBuffer;
    
    if(argc != 2)
    {
        fprintf(stdout, "usage: %s server_ip_address\neg.%s 192.168.1.1\n", argv[0], argv[0]);
        return 0;
    }
    serverIP = argv[1];

    SocketAddr addr(serverIP.c_str(), 30000, SocketAddr::Ipv4);
    Client client(loop);

    client.connectToServer(addr);

    //sendMessageThread(client);
    //sendMessageThread_2(addr);
    //client.SendMesgThread();
#if 0
    client.setConnectStatusCallback(
        [&client](uv::TcpClient::ConnectStatus status)
    {
        if(status == uv::TcpClient::ConnectStatus::OnConnectSuccess)
        {
            string msg = "HUB Client connect...";
            uv::Packet packet;
            packet.pack(msg.c_str(), msg.length());

            client.write(packet.Buffer().c_str(), packet.PacketSize());
        } else {
            std::cout << "Error: HUB Client connect to server fail" << std::endl;
        }
    });

#endif

#if 0
    client.setMessageCallback(
        [&client](const char* data, ssize_t size)
    {
        std::cout << std::string(data, size) << std::endl;
        int c, i = 0;
        string msg;

        std::cout << "HUB-2 Client msg: ";
        do {
            c = getchar();
            if(c == '\n')
                break;
            msg += c;
            i++;
        } while(1);

        if(msg.empty())
            return;
        
        msg = "HUB-2 Client send msg: " + msg;
        //uv::Packet packet;
        //packet.pack(msg.c_str(), msg.length());

        //client.write(packet.Buffer().c_str(), packet.PacketSize());
        client.write(msg.c_str(), msg.length());
    });
#endif    

#if 0
    client.setMessageCallback(
        [&client](const char* data, ssize_t size)
    {
        std::cout << std::string(data, size) << std::endl;

        string msg = "time delay";
        client.SendMesg(msg.c_str(), msg.length());
        //client.write(msg.c_str(), msg.length());
    });
#endif    

#if 0
    //跨线程发送数据
    std::thread thread([&client]()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        //char* data = new char[4] {'t','e','s','t'};
#if 1
		std::string data = "key=value&key2=value2";
	    uv::PacketIR packetir;
	    
	    packetir.SetHead(uv::PacketIR::HUB, 
	                     uv::PacketIR::MSG_GET_NETWORK_TOPOLOGY, 
	                     uv::PacketIR::REQUEST,
	                     uv::PacketIR::RRUID_2,
	                     uv::PacketIR::PORT_6);

	    packetir.PackMessage(data, data.length());
	    std::cout << "封装 packet:" << std::endl;
	    std::cout << "\tGetPacket: " << packetir.GetPacket() << std::endl;
	    std::cout << "\tGetHead: " << packetir.GetHead() << std::endl;
	    std::cout << "\tGetType: " << packetir.GetType() << std::endl;
	    std::cout << "\tGetMsgID: " << packetir.GetMsgID() << std::endl;
	    std::cout << "\tGetState: " << packetir.GetState() << std::endl;
	    std::cout << "\tGetRRUID: " << packetir.GetRRUID() << std::endl;
	    std::cout << "\tGetPort: " << packetir.GetPort() << std::endl;
	    std::cout << "\tGetLength: " << packetir.GetLength() << std::endl;
	    std::cout << "\tGetData: " << packetir.GetData() << std::endl;
	    std::cout << "\tData Length: " << data.length() << std::endl;

		std::string send_buf = packetir.GetPacket();
		//char* data = new char[send_buf.length()]{};	
		//strcpy(data,send_buf.c_str());
		std::cout << "send_buf=" << send_buf << std::endl;
		//std::cout << "data=" << data << std::endl;
		client.write(send_buf.c_str(), send_buf.length());
#endif   
#if 0
        //线程安全;
        client.writeInLoop(send_buf.c_str(), send_buf.length(),
        //client.writeInLoop(data, sizeof(data),
            [](uv::WriteInfo& info)
        {
            //数据需要在发生完成回调中释放
            //write message error.
            if (0 != info.status)
            {
                //打印错误信息
                std::cout << "Write error ：" << EventLoop::GetErrorMessage(info.status) << std::endl;
            }
            delete[] info.buf;
        });
#endif
    });
#endif

#if 0
    loop->runInThisLoop(
        [&client]()
    {
        std::cout << "run funciton in loop thread one." << endl;
        //string msg = "time delay";
        char* msg = new char[4] {'1', '2', '3', '4'};
        client.SendMesg(msg, sizeof(msg));
        //client.writeInLoop(msg, sizeof(msg), nullptr);
    });
#endif

    loop->run();


    return 0;
}

