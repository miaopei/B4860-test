#include "ServerCenter.h"
#include "../message_define/Message.h"
#include "UVThreadMng.h"
#include "LogMng.h"


ServerCenter::ServerCenter(void)
    : CUVThread(SERVER_CENTER_TYPE)
{
}


ServerCenter::~ServerCenter(void)
{
}

void ServerCenter::OnUvThreadMessage( CMsg msg, unsigned int nSrcAddr )
{
    std::cout << "ServerCenter::OnUvThreadMessage:m_nSessionID: " << msg.m_nSessionID << std::endl;
    for (int i = 0; i < 1000; i++)
    {
        msg.m_nTimes = i;

        CUVThreadMng::GetInstance()->SendUvMessage2Handle(msg, msg.MSG_ID, m_nThreadType);
    }
}
