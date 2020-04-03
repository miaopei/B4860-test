/*************************************************************************
	> File Name: bbu_server.h
	> Author: miaopei
	> Mail: miaopei@baicells.com 
	> Created Time: 2020年02月12日 星期三 11时15分58秒
 ************************************************************************/
#pragma once

#ifndef BBU_SERVER_H
#define BBU_SERVER_H 

#include "include/uv11.h"

class Server :public uv::TcpServer
{
public:
    Server(uv::EventLoop* loop);

private:
    void newMessage(uv::TcpConnectionPtr connection, const char* buf, ssize_t size); 
};

#endif // BBU_SERVER_H

