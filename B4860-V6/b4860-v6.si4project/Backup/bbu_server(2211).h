/*************************************************************************
	> File Name: bbu_server.h
	> Author: miaopei
	> Mail: miaopei@baicells.com 
	> Created Time: 2020年02月12日 星期三 11时15分58秒
 ************************************************************************/
#pragma once

#ifndef BBU_SERVER_H
#define BBU_SERVER_H 

#include <iostream>
#include <cstdio>
#include <cstring>
#include <memory>

#include "uv11.h"

class Server :public uv::TcpServer
{
public:
    Server(uv::EventLoop* loop);
    static void writeCallback(uv::WriteInfo& info);
    void WriteMessage(uv::TcpConnectionPtr connection, const char* buf, ssize_t size);
    void SendMessage(const char* buf, ssize_t size);

    void ReqMessageProcess(uv::TcpConnectionPtr connection, uv::PacketIR& packet);
    void ResMessageProcess(uv::TcpConnectionPtr connection, uv::PacketIR& packet);

	bool SetConnectionClient(uv::TcpConnectionPtr connection, uv::PacketIR packetir); 

    void NetworkTopology();

    void TestGet();

private:
    void OnMessage(uv::TcpConnectionPtr connection, const char* buf, ssize_t size); 
};

#endif // BBU_SERVER_H

