/*
   Copyright © 2017-2019, orcaer@yeah.net  All rights reserved.

   Author: orcaer@yeah.net

   Last modified: 2019-9-20

   Description: https://github.com/wlgq2/uv-cpp
*/

#include <thread>
#include <iostream>
#include <chrono>

#include "RpcCtrl.h"

using namespace std;

void Rpcreq(uv::EventLoop* loop, RpcClient& client)
{
    RpcCtrl rpc(loop, client);
    Req req;
    for (auto i = 1; i <= 100; i++)
    {
        req.params.push_back(i);
    }

    Resp resp;
    rpc.sum(req, resp);
    std::cout << "1~100 sum :"<<resp.param << std::endl;
}

int main(int argc, char** args)
{
    //rpc client
    uv::EventLoop* loop = new uv::EventLoop();

    uv::SocketAddr addr("127.0.0.1", 30000, uv::SocketAddr::Ipv4);

    RpcClient client(loop, addr);

    //rpc contrl
    Rpcreq(loop, client);

    loop->run();

    return 0;
}
