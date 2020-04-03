
#include <iostream>
#include <cstring>
#include "include/PacketIR.h"

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

void uv::PacketIR::SetHead(Type type, MsgID msgID, State state, RRUID rruid, Port port)
{
	m_type = to_string(type);
	m_msgID = to_string(msgID);
	m_state = to_string(state);
	m_rruid = to_string(rruid);
	m_port = to_string(port);
}

std::string uv::PacketIR::num2str(int num)
{
    char ss[10];
    sprintf(ss, "%04d", num);
    return ss;
}

void uv::PacketIR::PackMessage(std::string& data, size_t size)
{
    if(size >= 10000)
    {
        std::cout << "The data is too big" << std::endl;
        return;
    }
	m_data = data;
    m_length = size;
	m_packet = std::string(GetHead() + num2str(size) + data);	 
}

void uv::PacketIR::UnPackMessage(std::string& packet)
{
	// 需要增加 packet 解析校验
	m_packet = packet;
	m_type = packet.substr(0, 1);
	m_msgID = packet.substr(1, 4);
	m_state = packet.substr(5, 1);
	m_rruid = packet.substr(6, 1);
	m_port = packet.substr(7, 1);
    m_length = std::stoi(packet.substr(8, 4));
	m_data = packet.substr(12);
}

std::string uv::PacketIR::GetPacket() 
{ 
	return m_packet;
}

std::string uv::PacketIR::GetHead() 
{ 
	return std::string(m_type + m_msgID + m_state + m_rruid + m_port); 
}

std::string uv::PacketIR::GetType() 
{ 
	return m_type; 
}

std::string uv::PacketIR::GetMsgID() 
{ 
	return m_msgID; 
}

std::string uv::PacketIR::GetState()
{
	return m_state;
}

std::string uv::PacketIR::GetRRUID()
{
	return m_rruid;
}

std::string uv::PacketIR::GetPort()
{
	return m_port;
}

int uv::PacketIR::GetLength()
{
    return m_length;
}

std::string uv::PacketIR::GetData() 
{
	return m_data;
}

void uv::PacketIR::pack(const char* data, uint16_t size)
{
    dataSize_ = size;
    buffer_.resize(size+ PacketMinSize());

    buffer_[0] = HeadByte;
    PackNum(&buffer_[1], size);

    std::copy(data, data + size, &buffer_[sizeof(HeadByte) + sizeof(dataSize_)]);
    buffer_.back() = EndByte;
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
    return 10;
}
