/*************************************************************************
	> File Name: main.cpp
	> Author: miaopei
	> Mail: miaopei@baicells.com 
	> Created Time: 2020年02月12日 星期三 11时16分50秒
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <cstring>

#include "bbu_server.h"

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

int main(int argc, char *argv[])
{
    EventLoop* loop = EventLoop::DefaultLoop();

    //uv::GlobalConfig::BufferModeStatus = uv::GlobalConfig::CycleBuffer;

    SocketAddr addr("0.0.0.0", 30000, SocketAddr::Ipv4);

    Server server(loop);

    // 心跳超时
    server.setTimeout(40);
    server.bindAndListen(addr);

#if 0
    // client 发消息到server处理
    server.setMessageCallback(
        [](uv::TcpConnectionPtr conn, const char* data, ssize_t size)
    {
        std::cout << std::string(data, size) << std::endl;

        std::string str = std::string(data, size);
        int pos = str.find(':');
        str = str.substr(pos+1);
        str = ClearHeadTailSpace(str);

        std::cout << "sbustr: " << str;
        std::cout << " sizeof: " << str.length() << '\n' << std::endl;

        conn->write(str.c_str(), str.length(), nullptr);
        //conn->write(data, size, nullptr);
        //string msg = "BBU Server Send Message...";
        //conn->write(msg.c_str(), msg.length(), nullptr);
    });
#endif

#if 1
    // server 发消息到 client
    server.setMessageCallback(
        [](uv::TcpConnectionPtr conn, const char* data, ssize_t size)
    {
        //while(1)
        {
            std::cout << std::string(data, size) << '\n' << std::endl;
#if 0
            int c, i = 0;
            string msg;

            std::cout << "BBU send msg: ";
            do {
                c = getchar();
                if(c == '\n')
                    break;
                msg += c;
                i++;
            } while(1);
#endif
            string msg = "123abc";
            msg = "BBU Server send msg: " + msg;
            uv::Packet packet;
            packet.pack(msg.c_str(), msg.length());

            conn->writeInLoop(packet.Buffer().c_str(), packet.PacketSize(), nullptr);
        }
    });
#endif

    loop->run();

    return 0;
}

