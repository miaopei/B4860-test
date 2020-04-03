
#ifndef  UV_PACKETIR_H
#define  UV_PACKETIR_H

#include <string>
#include <sstream>
#include <iomanip>
#include "PacketBuffer.h"

//PacketIR:
//------------------------------------------------------------------------------------------
//  head  |  type  |  msgID  |  state   |  RRUID   |   PORT   |  length  |   data  |  end   |
// 1 Byte | 1 Byte | 2 Byte  |  1 Byte  |  1 Byte  |  1 Byte  |  4 Byte  |  N Byte | 1 Byte |
//------------------------------------------------------------------------------------------
// head: 数据包头校验 (0x7e)
// type：HUB、RRU、OAM (1,2,3)
// msgID：消息编号 (1001)
// state: 请求、响应 (1,2)
// RRUID: rruid为4 (4)   
// PORT: 上联HUB 2端口 (2)
// length: 0021 (data 数据长度)
// data: 用户数据
// end: 数据包结尾校验 (0xe7)

namespace uv
{

class PacketIR
{
public:

    enum MsgID
    {
        MSG_BEGIN   				= 1000,
        MSG_GET     				= 1001,
        MSG_SET     				= 1002,
        MSG_UPGRADE 				= 1003,
        MSG_GET_LOG					= 1004,
        MSG_GET_NETWORK_TOPOLOGY	= 1005,
        MSG_CONNECT					= 1040,
        MSG_DELAY_MEASUREMENT 		= 1041,
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
		RRUID_4		= 4,
        RRUID_X     = 9
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
		PORT_7	= 7,
        PORT_X  = 9
	};

    struct PackHead 
    {
        char t_type[1];
        char t_msgID[4];
        char t_state[1];
        char t_RRUID[1];
        char t_PORT[1];
    };

    PacketIR();
    ~PacketIR();

    static int readFromBuffer(PacketBuffer*, PacketIR&);

    void pack(const char* data, uint16_t size);
    
    void SetHead(Type type, MsgID msgID, State state, RRUID rruid, Port port);

    std::string num2str(int num);
    void PackMessage(std::string& data, size_t size);

    void UnPackMessage(std::string& packet);

    std::string GetPacket();
    std::string GetHead();
    std::string GetType();
    std::string GetMsgID();
	std::string GetState();
	std::string GetRRUID();
	std::string GetPort();
    int GetLength();
    std::string GetData();
	
    const char* getData();
    const uint16_t DataSize();
    const std::string& Buffer();
    const uint32_t PacketSize();

    void swap(std::string& str);

    static std::string dec2hex(int dec);

    template<typename NumType>
    static void UnpackNum(const uint8_t* data, NumType& num);

    template<typename NumType>
    static void PackNum(char* data, NumType num);

    static uint32_t PacketMinSize();

	//friend std::ostream& operator<<(std::ostream& os, const PacketIR& head);
    //friend std::istream& operator>>(std::istream& is, PacketIR& head);

public:
    enum DataMode
    {
        BigEndian,
        LittleEndian
    };

    static uint8_t HeadByte;
    static uint8_t EndByte;
    static DataMode Mode;

private:
    std::string m_type;
    std::string m_msgID;
	std::string m_state;
	std::string m_rruid;
	std::string m_port;
    int m_length;
    std::string m_data;
    std::string m_packet;

    std::string buffer_;
    uint16_t dataSize_;
};

#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

inline std::string PacketIR::dec2hex(int dec) //将int转成16进制字符串
{
    stringstream ioss; //定义字符串流
    string s_temp; //存放转化后字符
    //ioss << setiosflags(ios::uppercase) << hex << i; //以十六制(大写)形式输出
    ioss << resetiosflags(ios::uppercase) << hex << dec; //以十六制(小写)形式输出//取消大写的设置
    ioss >> s_temp;
    return std::string("0x" + s_temp);
}

template<typename NumType>
inline void PacketIR::UnpackNum(const uint8_t* data, NumType& num)
{
    num = 0;
    auto size = static_cast<int>(sizeof(NumType));
    if (PacketIR::DataMode::BigEndian == PacketIR::Mode)
    {
        for (int i = 0; i < size; i++)
        {
            num <<= 8;
            num |= data[i];

        }
    }
    else
    {
        for (int i = size-1; i >= 0; i--)
        {
            num <<= 8;
            num |= data[i];
        }
    }
}

template<typename NumType>
inline void PacketIR::PackNum(char* data, NumType num)
{
    int size = static_cast<int>(sizeof(NumType));
    if (PacketIR::DataMode::BigEndian == PacketIR::Mode)
    {
        for (int i = size-1; i >= 0; i--)
        {
            data[i] = num & 0xff;
            num >>= 8;
        }
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            data[i] = num & 0xff;
            num >>= 8;
        }
    }
}

#if 0
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
#endif

}
#endif
