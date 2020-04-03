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

#include "uv11.h"


class HUB :public uv::TcpClient
{
public:
    HUB(uv::EventLoop* loop);

    void connectToServer(uv::SocketAddr& addr);
    void reConnect();
    void sendTestMessage();
    void onConnect(ConnectStatus status);
    void newMessage(const char* buf, ssize_t size);
    void RecvMesg(const char* buf, ssize_t size);
    void SendMesg(const char* buf, ssize_t size);
    void SendMesgThread();
private:
    std::shared_ptr<uv::SocketAddr> sockAddr;
    std::shared_ptr<HUB> clientptr_;
};

#endif // TEST_CLIENT_H
