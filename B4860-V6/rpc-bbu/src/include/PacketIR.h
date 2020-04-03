/*
Copyright © 2017-2019, orcaer@yeah.net  All rights reserved.

Author: orcaer@yeah.net

Last modified: 2018-7-18

Description: https://github.com/wlgq2/uv-cpp
*/

#ifndef  UV_PACKETIR_H
#define  UV_PACKETIR_H

#include <string>
#include "PacketBuffer.h"

//PacketIR:
//-----------------------------------------------------------------------------------------
//  head  |  type  |  msgID  |  state   |  RRUID   |   PORT   |  length  |  data  |  end   |
// 1 Byte | 1 Byte | 4 Byte  |  1 Byte  |  1 Byte  |  1 Byte  |  2 Byte  | N Byte | 1 Byte |
//-----------------------------------------------------------------------------------------
// head: 数据包头校验 ()
// type：HUB、RRU、OAM (0,1,2)
// msgID：消息编号 (1001)
// state: 请求、响应 (0,1)
// RRUID: rruid为4 (4)   
// PORT: 上联HUB 2端口 (2)
// length: data 大小
// data: 用户数据
// end: 数据包结尾校验 ()

namespace uv
{

class PacketIR
{
public:
    PacketIR();
    ~PacketIR();

    static int readFromBuffer(PacketBuffer*, PacketIR&);

    void pack(const char* msgID, const char* type, const char* RRUID, const char* data, uint16_t size);

    const char* getData();
    const uint16_t DataSize();
    const std::string& Buffer();
    const uint32_t PacketSize();

    void swap(std::string& str);
    template<typename NumType>
    static void UnpackNum(const uint8_t* data, NumType& num);

    template<typename NumType>
    static void PackNum(char* data, NumType num);

    static uint32_t PacketMinSize();

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
    std::string buffer_;
    uint16_t dataSize_;
};

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
}
#endif
