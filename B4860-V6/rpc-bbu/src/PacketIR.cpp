/*
Copyright © 2017-2019, orcaer@yeah.net  All rights reserved.

Author: orcaer@yeah.net

Last modified: 2019-10-24

Description: https://github.com/wlgq2/uv-cpp
*/

#include  "include/PacketIR.h"

using namespace uv;


uint8_t PacketIR::HeadByte = 0x7e;
uint8_t PacketIR::EndByte = 0xe7;
PacketIR::DataMode PacketIR::Mode = PacketIR::DataMode::LittleEndian;

PacketIR::PacketIR()
    :buffer_(""),
    dataSize_(0)
{

}

uv::PacketIR::~PacketIR()
{

}

int uv::PacketIR::readFromBuffer(PacketBuffer* packetbuf, PacketIR& out)
{
    std::string data("");
    while (true)
    {
        auto size = packetbuf->readSize();
        //数据小于包头大小
        if (size < PacketMinSize())
        {
            return -1;
        }
        //找包头
        uint16_t dataSize;
        packetbuf->readBufferN(data, sizeof(dataSize)+1);
        if ((uint8_t)data[0] != HeadByte) //包头不正确，从下一个字节开始继续找
        {
            data.clear();
            packetbuf->clearBufferN(1);
            continue;
        }
        UnpackNum((uint8_t*)data.c_str() + 1, dataSize);
        uint16_t msgsize = dataSize + PacketMinSize();
        //包不完整
        if (size < msgsize)
        {
            return -1;
        }
        packetbuf->clearBufferN(sizeof(dataSize)+1);
        packetbuf->readBufferN(data, dataSize +1);
        //检查包尾
        if ((uint8_t)data.back() == EndByte)
        {
            packetbuf->clearBufferN(dataSize +1);
            break;
        }
    }
    out.swap(data);
    return 0;
}

void uv::PacketIR::pack(const char* msgID, const char* type, const char* RRUID, const char* data, uint16_t size)
{
    dataSize_ = size;
    
}

const char* uv::PacketIR::getData()
{
    return buffer_.c_str()+sizeof(HeadByte)+sizeof(dataSize_);
}

const uint16_t uv::PacketIR::DataSize()
{
    return dataSize_;
}

const std::string& uv::PacketIR::Buffer()
{
    return buffer_;
}

const uint32_t uv::PacketIR::PacketSize()
{
    return (uint32_t)buffer_.size();
}

void uv::PacketIR::swap(std::string& str)
{
    buffer_.swap(str);
    dataSize_ = (uint16_t)(buffer_.size() - PacketMinSize());
}


uint32_t uv::PacketIR::PacketMinSize()
{
    return 17;
}
