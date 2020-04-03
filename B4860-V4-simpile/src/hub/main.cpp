#include <iostream>
#include <string>
#include "hub_client.h"
#include "UVThread.h"
#include "LogMng.h"
using namespace std;

std::string serverip;
int call_time = 0;
bool is_exist = false;

void OnInstanceExit()
{
    CLogMng::GetInstance()->StopLog();
}

int main(int argc, char** argv)
{
    //::atexit(OnInstanceExit);

    //CLogMng::GetInstance();

    if (argc != 3) {
        fprintf(stdout, "usage: %s server_ip_address clientcount\neg.%s 192.168.1.1 50\n", argv[0], argv[0]);
        return 0;
    }
    serverip = argv[1];

    HubClient pClients;

    std::cout << "serverip: " << serverip << std::endl;

    if (!pClients.Connect(serverip.c_str(), 30000)) {
        fprintf(stdout, "connect error:%s\n", pClients.GetLastErrMsg());
    } else {
        fprintf(stdout, "client(%p) connect succeed.\n", &pClients);
    }

    //pClients.SetKeepAlive(1,60);

    CMsg msg;
    msg.m_nSessionID = 666;
    //msg.m_nTimes = 888;
    pClients.SendUvMessage(msg, msg.MSG_ID);
    while (!is_exist) {
        uv_thread_sleep(10);
    }
    return 0;
}

