/*************************************************************************
	> File Name: hub_client.h
	> Author: miaopei
	> Mail: miaopei@baicells.com 
	> Created Time: 2020年02月12日 星期三 11时16分50秒
 ************************************************************************/

#pragma once

#ifndef TEST_CLIENT_H
#define TEST_CLIENT_H

#include <string>

#include "uv/include/uv11.h"

class Client :public uv::TcpClient
{
public:
    Client(uv::EventLoop* loop);

    void connectToServer(uv::SocketAddr& addr);
    void reConnect();
    void sendTestMessage();
    void onConnect(ConnectStatus status);
    void newMessage(const char* buf, ssize_t size);
private:
    std::shared_ptr<uv::SocketAddr> sockAddr;
};

#endif // TEST_CLIENT_H
