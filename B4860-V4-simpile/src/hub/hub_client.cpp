#include "hub_client.h"
#include "log4z.h"

HubClient::HubClient()
{
}

HubClient::~HubClient()
{
}

int HubClient::OnUvMessage(const CMsg &msg, TcpClientCtx *pClient)
{
    LOGI("msg.m_nTimes = " << msg.m_nTimes);
    std::cout << "msg.m_nTimes = " << msg.m_nTimes << std::endl;

    //return 0;
    return this->SendUvMessage(msg, msg.MSG_ID);
}
