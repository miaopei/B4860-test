/*
   Copyright © 2017-2019, orcaer@yeah.net  All rights reserved.

   Author: orcaer@yeah.net

   Last modified: 2019-9-20

   Description: https://github.com/wlgq2/uv-cpp
*/

#include <thread>
#include <iostream>
#include <chrono>

#include "RpcServer.h"

using namespace std;

void runRpcServer(uv::SocketAddr& addr)
{
    thread back([&addr]()
    {
        uv::EventLoop loop;
        RpcServer server(&loop);
        server.bindAndListen(addr);
        loop.run();
    });
    back.detach();
    this_thread::sleep_for(chrono::milliseconds(200));
}

int main(int argc, char** args)
{
    //rpc server
    //runRpcServer(addr);
    uv::EventLoop* loop = new uv::EventLoop();

    uv::SocketAddr addr("127.0.0.1", 30000);

    RpcServer server(loop);
    server.bindAndListen(addr);

    loop->run();

    return 0;
}
