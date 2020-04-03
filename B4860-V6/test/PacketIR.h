#pragma once

#include <iostream>
#include <string>

using namespace std;

namespace uv 
{

class PacketIR
{
public:	
    enum MsgID
    {
        MSG_BEGIN   			= 1000,
        MSG_GET     			= 1001,
        MSG_SET     			= 1002,
        MSG_UPGRADE 			= 1003,
        MSG_GET_LOG				= 1004,
        MSG_DELAY_MEASUREMENT 	= 1040,
        MSG_END
    };

    enum Type
    {
        HUB     = 0,
        RRU     = 1,
        OAM     = 2
    };

    enum State
    {
        REQUEST     = 0,
        RESPONSE    = 1 
    };

	enum RRUID
	{
		RRUID_0 	= 0,
		RRUID_1		= 1,
		RRUID_2	 	= 2,
		RRUID_3		= 3,
		RRUID_4		= 4
	};

	enum Port
	{
		PORT_0	= 0,
		PORT_1	= 1,
		PORT_2 	= 2,
		PORT_3 	= 3,
		PORT_4	= 4,
		PORT_5	= 5,
		PORT_6	= 6,
		PORT_7	= 7
	};
	
    PacketIR() {}
#if 0
    PacketIR(std::string type, std::string msgID)
        : m_type(type),
        m_msgID(msgID)
    {

    }
#endif
    ~PacketIR() {}

    bool SetHead(Type type, MsgID msgID)
    {
   	#if 0
        if(type.length() != 1 || msgID.length() != 4)
        {
            std::cout << "Error: SetHead Parameter Byte Error!" << std::endl;
            return false;
        }
	#endif
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
        // 需要增加 packet 解析校验
        m_packet = packet;
        m_type = packet.substr(0, 1);
        m_msgID = packet.substr(1, 4);
        m_data = packet.substr(5);
    }

    std::string GetPacket() { return m_packet; }
    std::string GetHead() { return std::string(m_type + m_msgID); }
    std::string GetType() { return m_type; };
    std::string GetMsgID() { return m_msgID; }
    std::string GetData() { return m_data; }

    friend std::ostream& operator<<(std::ostream& os, const PacketIR& head);
    friend std::istream& operator>>(std::istream& is, PacketIR& head);

private:
    std::string m_type;
    std::string m_msgID;
    std::string m_data;
    std::string m_packet;
};

std::ostream& operator<<(std::ostream& os, const PacketIR& head)
{
    return os << head.m_type << head.m_msgID;
}

std::istream& operator>>(std::istream& is, PacketIR& head)
{
    std::string str;
    is >> str;
    head.m_type = str.substr(0, 1);
    head.m_msgID = str.substr(1, 4);
    return is; 
}

}
