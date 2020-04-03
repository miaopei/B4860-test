
#ifndef  UV_PACKETIR_H
#define  UV_PACKETIR_H

#include <string>
#include <sstream>
#include <iomanip>
#include "PacketBuffer.h"

#define HEADLENGTH 		14

//PacketIR:
//-------------------------------------------------------------------------------------------------------
//  head  | source | destination | state  | msgID  | RRUID  |  port  | uPort  | length |  data  |  end   |
// 1 Byte | 1 Byte |    1 Byte   | 1 Byte | 4 Byte | 1 Byte | 1 Byte | 1 Byte | 4 Byte | N Byte | 1 Byte |
//-------------------------------------------------------------------------------------------------------
// head: 数据包头校验 (0x7e)
// source：HUB、RRU、BBU、OAM (0,1,2,3)
// destination: 消息目标发给谁 HUB、RRU、BBU、OAM 
// state: 请求、响应 (0,1)
// msgID：消息编号 (1001)
// RRUID: rruid为4 (4)   
// port: 设备本端端口号 (0,1)
// uPort: 设备连接对端端口号 (0-7)
// length: 0021 (data 数据长度)
// data: 用户数据
// end: 数据包结尾校验 (0xe7)
// eg: (head 14 Byte)
//      HUB MSG_CONNECT REQUEST TO_BBU RRUID_2 PORT_1 UPORT_4 length data
//      0   1040        0       2      2       1      4       0023   key0=value0&key1=value1
//      01040022140023key0=value0&key1=value1

namespace uv
{

class PacketIR
{
public:

    enum Source
    {
        HUB     = 0,
        RRU     = 1,
        BBU     = 2,
        OAM     = 3
    };

    enum Destination
    {
        TO_HUB     = 0,
        TO_RRU     = 1,
        TO_BBU     = 2,
        TO_OAM     = 3
    };

    enum State
    {
        REQUEST     = 0,
        RESPONSE    = 1 
    };

    enum MsgID
    {
        MSG_BEGIN   				= 1000,
        MSG_GET_KEY     			= 1001,
        MSG_GET_MSGID               = 1002,
        MSG_SET     				= 1003,
        MSG_UPGRADE 				= 1004,
        MSG_GET_LOG					= 1005,
        MSG_GET_NETWORK_TOPOLOGY	= 1006,
        MSG_CONNECT					= 1040,
        MSG_DELAY_MEASUREMENT 		= 1041,
        MSG_END                     = 9999
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
		PORT_8	= 8,
        PORT_X  = 9
	};

    enum UPort
	{
		UPORT_0	    = 0,
		UPORT_1	    = 1,
		UPORT_2 	= 2,
		UPORT_3 	= 3,
		UPORT_4	    = 4,
		UPORT_5	    = 5,
		UPORT_6	    = 6,
		UPORT_7	    = 7,
		UPORT_8	    = 8,
        UPORT_X     = 9
	};

    /* struct PackHead */ 
    /* { */
    /*     char t_type[1]; */
    /*     char t_msgID[4]; */
    /*     char t_state[1]; */
    /*     char t_RRUID[1]; */
    /*     char t_PORT[1]; */
    /* }; */

    PacketIR();
    ~PacketIR();

    static int readFromBuffer(PacketBuffer*, PacketIR&);

    void pack(const char* data, uint16_t size);
    
    void SetHead(Source sour, Destination dest, State state, MsgID msgID, RRUID rruid, Port port, UPort uport);

    std::string num2str(int num);
    void PackMessage(std::string& data, size_t size);

    void UnPackMessage(std::string& packet);

    std::string GetPacket();
    std::string GetHead();
    std::string GetSource();
	std::string GetDestination();
	std::string GetState();
    std::string GetMsgID();
	std::string GetRRUID();
	std::string GetPort();
	std::string GetUPort();
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
    std::string m_source;
	std::string m_destination;
	std::string m_state;
    std::string m_msgID;
	std::string m_rruid;
	std::string m_port;
	std::string m_uport;
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
