/*************************************************************************
	> File Name: bbu.h
	> Author: miaopei
	> Mail: miaopei@baicells.com 
	> Created Time: 2020年02月12日 星期三 11时15分58秒
 ************************************************************************/
#pragma once

#ifndef BBU_SERVER_H
#define BBU_SERVER_H 

#include <iostream>
#include <cstdio>
#include <cstring>

#include "uv11.h"

class BBU :public uv::TcpServer
{
public:
    BBU(uv::EventLoop* loop);
    //static void writeCallback(uv::WriteInfo& info);
    void SendMessage(uv::TcpConnectionPtr connection, const char* buf, ssize_t size);
	void SendPackMessage(uv::TcpConnectionPtr& connection, uv::PacketIR& packet);
    //void SendAllClientMessage(const char* buf, ssize_t size);

	void BBUMessageProcess(uv::TcpConnectionPtr& connection, uv::PacketIR& packet);
	void HUBMessageProcess(uv::TcpConnectionPtr& connection, uv::PacketIR& packet);
	void RRUMessageProcess(uv::TcpConnectionPtr& connection, uv::PacketIR& packet);
	void OAMMessageProcess(uv::TcpConnectionPtr& connection, uv::PacketIR& packet);

	void SetConnectionClient(uv::TcpConnectionPtr& connection, uv::PacketIR& packet);
	void DelayMeasurementProcess(uv::TcpConnectionPtr& connection, uv::PacketIR& packet);

	void UnPackData(uv::PacketIR& packet);
	
    /* 时延测量需要实现的几个接口函数 */
    /* 更新HUB时延测量信息 */
    void UpdateHUBDelayInfo(uv::PacketIR& packet);
    /* 时延补偿计算，整个链路如何实现自动计算？补偿计算值排序，最大时延支持可配置 */
    void CalculationDelayCompensation();
    /* HUB 时延信息存储map维护，设备掉电需要更新map RRU新接入需要更新 */
    void HubDelayInfo();

    void NetworkTopology();

    void TestGet();

private:
    void OnMessage(uv::TcpConnectionPtr connection, const char* buf, ssize_t size); 
};

#endif // BBU_SERVER_H

