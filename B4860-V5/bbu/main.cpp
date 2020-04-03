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
    //server.setTimeout(40);
    server.bindAndListen(addr);

#if 0
    // server 发消息到 client
    server.setMessageCallback(
        [&server](uv::TcpConnectionPtr conn, const char* data, ssize_t size)
    {
        std::cout << std::string(data, size) << std::endl;
        //string sendmsg = "BBU Server send msg all client.";
        string sendmsg = "666";
        server.SendMsg(sendmsg.c_str(), sendmsg.length());
        //conn->write(sendmsg.c_str(), sendmsg.length(), nullptr);
    });
#endif

    string sendmsg = "666";
    server.SendMsg(sendmsg.c_str(), sendmsg.length());

    uint8_t type = 1;
    uint16_t msgID = uv::PacketIR::MsgID::MSG_GET;
    uint8_t state = uv::PacketIR::State::REQUEST;
    uint8_t rruid = 4; 
    uint8_t port = 2;
    uv::PacketIR packet;
    packet.SetMessageHead(type, msgID, state, rruid, port);
    packet.pack(sendmsg.c_str(), sendmsg.length());
    std::cout << "data=" << packet.Buffer() << std::endl;
    std::cout << "data[0]=" << (uint8_t)packet.Buffer()[0] << std::endl;
    std::cout << "size=" << packet.PacketSize() << std::endl;
    std::cout << "type=" << (int)packet.GetType() << std::endl;

    string testmsg = "123abc";
    uv::Packet testpacket;
    testpacket.pack(testmsg.c_str(), testmsg.length());
    std::cout << "data=" << testpacket.Buffer() << std::endl;
    std::cout << "size=" << testpacket.PacketSize() << std::endl;

#if 0
    //跨线程发送数据
    std::thread thread([&server]()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        char* data = new char[4] {'t','e','s','t'};
#if 1
        std::map<std::string ,TcpConnectionPtr>  allconnnections;
        server.getAllConnection(allconnnections);

        for(auto &conn : allconnnections)
        {
            std::cout << "client ip=" << conn.first << std::endl; 
            //conn.second->write(msg, size, nullptr);
            //WriteMesg(conn.second, msg, size);

        //线程安全;
        conn.second->writeInLoop(data,sizeof(data),
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
        }
#endif
        //string sendmsg = "BBU Server send msg all client.";
        //server.SendMsg(sendmsg.c_str(), sendmsg.length());
    });
#endif

#if 0
    server.setMessageCallback(
        [&server](uv::TcpConnectionPtr conn, const char* data, ssize_t size)
    {
        std::cout << std::string(data, size) << std::endl;
        int c, i = 0;
        string msg;

        std::cout << "BBU Server msg: ";
        do {
            c = getchar();
            if(c == '\n')
                break;
            msg += c;
            i++;
        } while(1);

        if(msg.empty())
            return;
        
        msg = "BBU Server send msg: " + msg;
        server.SendMsg(msg.c_str(), msg.length());
    });

#endif

    loop->run();

    return 0;
}

