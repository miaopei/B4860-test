#ifndef MSGID_H_  
#define MSGID_H_

#include <iostream>

namespace std;

namespace uv
{

class Msg
{
public:
    enum
    {
        MSG_BEGIN = 0,
        MSG_GET = 1,
        MSG_SET = 2,
        MSG_END
    };
    Msg()
        : m_msgID(auto MSG_BEGIN)
    {

    }

    string m_msgID;
    string m_type; // oam hub rru
    string m_RRUID;
    string m_length;
    string m_data;
};

}

#endif