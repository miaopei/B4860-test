#include "GateWay.h"
#include "UVThreadMng.h"
#include "LogMng.h"

GateWay::GateWay()
    : m_nSessionID(0)
{
}

GateWay::~GateWay()
{
}

int GateWay::OnUvMessage(const CMsg &msg, TcpClientCtx *pClient)
{
    CMsg test = msg;
    test.m_nSessionID = m_nSessionID++;

    m_mapSession[test.m_nSessionID] = pClient;

    std::cout << "GateWay::OnUvMessage:m_nSessionID: " << test.m_nSessionID << " m_nTimes: " << test.m_nTimes << std::endl;
    std::cout << "GateWay::OnUvMessage:MSG_ID: " << test.MSG_ID << std::endl;

    return CUVThreadMng::GetInstance()->SendUvMessage(test, test.MSG_ID, SERVER_CENTER_TYPE);
}

void GateWay::OnUvThreadMessage( CMsg msg, unsigned int nSrcAddr )
{
    LOGI("msg.m_nTimes = " << msg.m_nTimes);
    map<int, TcpClientCtx *>::iterator it = m_mapSession.find(msg.m_nSessionID);

    std::cout << "GateWay::OnUvThreadMessage:m_nSessionID: " << msg.m_nSessionID << " m_nTimes: " << msg.m_nTimes << std::endl;
    std::cout << "GateWay::OnUvThreadMessage:MSG_ID: " << msg.MSG_ID << std::endl;

    if (it != m_mapSession.end())
    {
        this->SendUvMessage(msg, msg.MSG_ID, it->second);
        // m_mapSession.erase(it);
    }
}
