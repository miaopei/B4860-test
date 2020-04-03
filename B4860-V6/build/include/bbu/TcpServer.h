/*
   Copyright © 2017-2019, orcaer@yeah.net  All rights reserved.

   Author: orcaer@yeah.net

   Last modified: 2019-12-31

   Description: https://github.com/wlgq2/uv-cpp
*/

#ifndef UV_TCP_SERVER_H
#define UV_TCP_SERVER_H

#include <functional>
#include <memory>
#include <set>
#include <map>
#include <vector>


#include "TcpAccepter.h"
#include "TcpConnection.h"
#include "TimerWheel.h"
#include "PacketIR.h"

namespace uv
{

using OnConnectionStatusCallback =  std::function<void (std::weak_ptr<TcpConnection> )> ;

//no thread safe.
class TcpServer
{
public:
	struct ClientInfo
    {
    	std::string s_ip;
    	TcpConnectionPtr s_connection;
        std::string s_type;
        std::string s_RRUID;
        std::string s_port;
        ClientInfo(){};
        ClientInfo(std::string ip, TcpConnectionPtr      connect, std::string type, std::string rruid, std::string port)
        {
        	s_ip = ip;
			s_connection = connect;
            s_type = type;
            s_RRUID = rruid;
            s_port = port;
        };
    };
	
    TcpServer(EventLoop* loop, bool tcpNoDelay = true);
    virtual ~TcpServer();
    int bindAndListen(SocketAddr& addr);
    void addConnnection(std::string& name,TcpConnectionPtr connection);
    void removeConnnection(std::string& name);
    TcpConnectionPtr getConnnection(std::string& name);
    void closeConnection(std::string& name);
    void getAllConnection(std::map<std::string ,TcpConnectionPtr>  &allconnnections);

    void onMessage(TcpConnectionPtr connection,const char* buf,ssize_t size);
    void setMessageCallback(OnMessageCallback callback);

    void setNewConnectCallback(OnConnectionStatusCallback callback);
    void setConnectCloseCallback(OnConnectionStatusCallback callback);

    void write(TcpConnectionPtr connection,const char* buf,unsigned int size, AfterWriteCallback callback = nullptr);
    void write(std::string& name,const char* buf,unsigned int size, AfterWriteCallback callback =nullptr);
    void writeInLoop(TcpConnectionPtr connection,const char* buf,unsigned int size,AfterWriteCallback callback);
    void writeInLoop(std::string& name,const char* buf,unsigned int size,AfterWriteCallback callback);

	std::string GetCurrentName(TcpConnectionPtr connection);
	bool SetConnectionInfo(TcpConnectionPtr connection, ClientInfo& cInfo);
	void GetHUBsConnection(std::vector<TcpConnectionPtr>& hubsConnection);
	void GetRRUsConnection(std::vector<TcpConnectionPtr>& rrusConnection);
	void GetNetworkTopology(std::map<std::string, ClientInfo>& netTopology);

    void setTimeout(unsigned int);
private:
    void onAccept(EventLoop* loop, UVTcpPtr client);
protected:
    EventLoop* loop_;
private:
    bool tcpNoDelay_;
    SocketAddr::IPV ipv_;
    std::shared_ptr <TcpAccepter> accetper_;
    std::map<std::string ,TcpConnectionPtr>  connnections_;
	ClientInfo cInfo_;
	std::map<std::string, ClientInfo> connectionInfo_;

    OnMessageCallback onMessageCallback_;
    OnConnectionStatusCallback onNewConnectCallback_;
    OnConnectionStatusCallback onConnectCloseCallback_;
    TimerWheel timerWheel_;
};


}
#endif
