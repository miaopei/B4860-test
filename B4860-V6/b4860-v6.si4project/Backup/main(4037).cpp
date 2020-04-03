/*************************************************************************
	> File Name: main.cpp
	> Author: miaopei
	> Mail: miaopei@baicells.com 
	> Created Time: 2020年02月12日 星期三 11时16分50秒
 ************************************************************************/

#include "bbu_server.h"

using namespace uv;

std::string serverIP;

int main(int argc, char *argv[])
{
    EventLoop* loop = EventLoop::DefaultLoop();

    //uv::GlobalConfig::BufferModeStatus = uv::GlobalConfig::CycleBuffer;
    
    if(argc != 2)
    {
        fprintf(stdout, "usage: %s server_ip_address\neg.%s 192.168.1.1\n", argv[0], argv[0]);
        return 0;
    }
    serverIP = argv[1];

    SocketAddr addr(serverIP.c_str(), 30000, SocketAddr::Ipv4);

    Server server(loop);

    // 心跳超时
    //server.setTimeout(40);
    server.bindAndListen(addr);

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

