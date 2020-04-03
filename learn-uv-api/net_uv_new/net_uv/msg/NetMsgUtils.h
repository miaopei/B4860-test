#pragma once

#include "NetMsgMgr.h"

NS_NET_UV_BEGIN


#define UV_ENABLE_MD5_CHECK 1
// 64mb
#define NET_UV_MAX_MSG_LEN (655350)

#define NET_UV_WRITE_MAX_LEN (1400)


struct NetMsgHead
{
	uint32_t len;// ��Ϣ���ȣ����������ṹ��

	enum NetMsgType
	{
		PING,
		PONG,
		MSG
	}type;
};

const static uint32_t NetMsgHeadLen = sizeof(NetMsgHead);

//����
char* net_uv_encode(MD5& M, const char* data, uint32_t len, uint32_t &outLen);
//����
char* net_uv_decode(MD5& M, const char* data, uint32_t len, uint32_t &outLen);

NS_NET_UV_END