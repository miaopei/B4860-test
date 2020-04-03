#pragma once

#include <iostream>
#include <string>

using namespace std;

namespace uv 
{

class Head
{
public:
    Head(std::string type, std::string msgID)
        : m_type(type),
        m_msgID(msgID)
    {

    }
    ~Head() {}

    bool SetHead(std::string type, std::string msgID)
    {
        if(type.length() != 1 && msgID.length() != 4)
        {
            std::cout << "Error: SetHead Parameter Byte Error!" << std::endl;
            return false;
        }
        m_type = type;
        m_msgID = msgID;
        return true;
    }

    void PackMessage(std::string& data, size_t size)
    {
        m_data = data;
        m_packet = std::string(GetHead() + data);    
    }

    void UnPackMessage(std::string& packet)
    {
        m_type = packet.substr(0, 1);
        m_msgID = packet.substr(1, 4);
        m_data = packet.substr(4);
    }

    std::string GetPacket() { return m_packet; }
    std::string GetHead() { return std::string(m_type + m_msgID); }
    std::string GetType() { return m_type; };
    std::string GetMsgID() { return m_msgID; }
    std::string GetData() { return m_data; }

    friend std::ostream& operator<<(std::ostream& os, const Head& head);
    friend std::istream& operator>>(std::istream& is, Head& head);

private:
    std::string m_type;
    std::string m_msgID;
    std::string m_data;
    std::string m_packet;
};

std::ostream& operator<<(std::ostream& os, const Head& head)
{
    return os << head.m_type << head.m_msgID;
}

std::istream& operator>>(std::istream& is, Head& head)
{
    std::string str;
    is >> str;
    head.m_type = str.substr(0, 1);
    head.m_msgID = str.substr(1, 4);
    return is; 
}

}
