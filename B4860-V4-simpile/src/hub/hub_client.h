#pragma once
#include <iostream>
#include "tcpclient.h"
#include "../message_define/Message.h"
class HubClient:
    public CTCPClient
{
public:
    HubClient();
    virtual ~HubClient();

protected:
    BEGIN_UV_BIND
        UV_BIND(CMsg::MSG_ID, CMsg)
    END_UV_BIND(CTCPClient)

    int OnUvMessage(const CMsg &msg, TcpClientCtx *pClient);
};

