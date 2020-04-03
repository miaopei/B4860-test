#pragma once
#include <iostream>
#include "UVThread.h"
#include "../message_define/Message.h"
class ServerCenter :
    public CUVThread
{
public:
    ServerCenter(void);
    virtual ~ServerCenter(void);

    BEGIN_UV_THREAD_BIND
        UV_THREAD_BIND(CMsg::MSG_ID, CMsg)
    END_UV_THREAD_BIND(CUVThread)

protected:
        void OnUvThreadMessage(CMsg msg, unsigned int nSrcAddr);
};

