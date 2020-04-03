#pragma once
#include <iostream>
#include "tcpserver.h"
#include "../message_define/Message.h"

class GateWay :
    public CTCPServer
{
public:
    GateWay();
    virtual ~GateWay();

protected:
    BEGIN_UV_BIND
        UV_BIND(CMsg::MSG_ID, CMsg)
    END_UV_BIND(CTCPServer)

    BEGIN_UV_THREAD_BIND
        UV_THREAD_BIND(CMsg::MSG_ID, CMsg)
    END_UV_THREAD_BIND(CTCPServer)

    int OnUvMessage(const CMsg &msg, TcpClientCtx *pClient);

    void OnUvThreadMessage(CMsg msg, unsigned int nSrcAddr);

    int m_nSessionID;

private:
    map<int, TcpClientCtx *> m_mapSession;
};

