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

#include "rru_client.h"

using namespace uv;
using namespace std;

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

int main(int argc, char* argv[])
{
    EventLoop* loop = new EventLoop();

    //uv::GlobalConfig::BufferModeStatus = uv::GlobalConfig::CycleBuffer;

    SocketAddr addr("127.0.0.1", 30000, SocketAddr::Ipv4);
    Client client(loop);

#if 0
    client.setConnectStatusCallback(
        [&client](uv::TcpClient::ConnectStatus status)
    {
        if(status == uv::TcpClient::ConnectStatus::OnConnectSuccess)
        {
            string msg = "RRU Client connect...";
            uv::Packet packet;
            packet.pack(msg.c_str(), msg.length());

            client.write(packet.Buffer().c_str(), packet.PacketSize());
        } else {
            std::cout << "Error: RRU Client connect to server fail" << std::endl;
        }
    });

#endif

#if 0
    // client 接收 server 消息
    client.setMessageCallback(
        [&client](const char* data, ssize_t size)
    {
        std::cout << std::string(data, size) << std::endl;

        std::string str = std::string(data, size);
        int pos = str.find(':');
        str = str.substr(pos+1);
        str = ClearHeadTailSpace(str);

        std::cout << "sbustr: " << str;
        std::cout << " sizeof: " << str.length() << '\n' << std::endl;

        string msg = "RRU recv msg: ";
        msg += str;
        client.write(msg.c_str(), msg.length());
        //client.write(str.c_str(), str.length());
    });
#endif

#if 0
    // client 发消息到server处理
    client.setConnectStatusCallback(
        [&client](uv::TcpClient::ConnectStatus status)
    {
        if(status == uv::TcpClient::ConnectStatus::OnConnectSuccess)
        {
            while(1)
            {
                int c, i = 0;
                string msg;

                std::cout << "RRU Client msg: ";
                do {
                    c = getchar();
                    if(c == '\n')
                        break;
                    msg += c;
                    i++;
                } while(1);

                msg = "RRU Client send msg: " + msg;
                uv::Packet packet;
                packet.pack(msg.c_str(), msg.length());

                client.write(packet.Buffer().c_str(), packet.PacketSize());
                //client.onMessage(conn, packet.Buffer().c_str(), packet.PacketSize());
            }
        } else {
            std::cout << "Error: RRU Client connect to server fail" << std::endl;
        }
    });
#endif

#if 1
    client.setMessageCallback(
        [&client](const char* data, ssize_t size)
    {
        std::cout << std::string(data, size) << std::endl;
        int c, i = 0;
        string msg;

        std::cout << "RRU Client msg: ";
        do {
            c = getchar();
            if(c == '\n')
                break;
            msg += c;
            i++;
        } while(1);

        if(msg.empty())
            return;
        
        msg = "RRU Client send msg: " + msg;
        //uv::Packet packet;
        //packet.pack(msg.c_str(), msg.length());

        //client.write(packet.Buffer().c_str(), packet.PacketSize());
        client.write(msg.c_str(), msg.length());
    });
#endif    

#if 0
    // client 发消息到server处理
    client.setConnectStatusCallback(
        [&client](uv::TcpClient::ConnectStatus status)
    {
        if(status == uv::TcpClient::ConnectStatus::OnConnectSuccess)
        {
            string msg;
            msg = "RRU Client send msg: qwe456";

            uv::Packet packet;
            packet.pack(msg.c_str(), msg.length());
            client.write(packet.Buffer().c_str(), packet.PacketSize());
        } else {
            std::cout << "Error: RRU Client connect to server fail" << std::endl;
        }
    });
#endif

    client.connectToServer(addr);
    loop->run();

    return 0;
}

