#ifndef MESSAGEHEAD_H_
#define MESSAGEHEAD_H_

#include <iostream>

namespace uv
{

class MessageHead
{
public:
    enum MsgID
    {
        MSG_BEGIN = 1000,
        MSG_GET,
        MSG_SET,
        MSG_END
    };
    
    enum State
    {
        REQUEST = 1,
        RESPONSE
    };
    
    MessageHead(uint8_t type, uint16_t msgID, uint8_t state, uint8_t rruid,uint8_t port)
        : m_type(type),
        m_msgID(msgID),
        m_state(state),
        m_RRUID(rruid),
        m_PORT(port)
    {

    }

    void GetMessageHead(char buf[]);

private:
    uint8_t m_type; // oam hub rru
    uint16_t m_msgID;
    uint8_t m_state; // req resp
    uint8_t m_RRUID;
    uint8_t m_PORT;
};

}

#endif
