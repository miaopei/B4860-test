/*************************************************************************
	> File Name: hub.h
	> Author: miaopei
	> Mail: miaopei@baicells.com 
	> Created Time: 2020年02月12日 星期三 11时16分50秒
 ************************************************************************/

#pragma once

#include <string>

#include "uv11.h"

#define TOFFSET 3

#if 0
#ifdef __cplusplus
extern "C" {
#endif

#include "rhub.h"

uint16_t get_rhup_port_id(int mpi_fd, uint8_t port);
void get_rhup_cpri_stat(int mpi_fd, uint8_t dir, uint8_t port, struct rhup_cpri_stat* cpri_stat);
void get_rhup_delay(int mpi_fd, uint8_t dir, struct rhup_data_delay* rhup_delay);
uint16_t get_rhup_port_stat(int mpi_fd);
void get_rhup_t14_delay(int mpi_fd, struct rhup_t14_delay* t14_delay);

int gpmc_mpi_open(const char* gpmc_mpi_dev);
void gpmc_mpi_close(int fd);
unsigned int gpmc_mpi_write(int fd,unsigned int addr,unsigned int wdata);
unsigned int gpmc_mpi_read(int fd,unsigned int addr);
unsigned int gpmc_mpi_write_device(int fd,unsigned int module_addr, unsigned int reg_addr, unsigned int reg_wdata);
unsigned int gpmc_mpi_read_device(int fd,unsigned int module_addr, unsigned int reg_addr);

#ifdef __cplusplus
}
#endif

struct delay_measurement_info {
    struct rhup_data_delay* rhup_delay;
    struct rhup_t14_delay* t14_delay;
};
#endif

struct rhub_data_delay{
    uint16_t delay1;
    uint16_t delay2;
};

struct rhub_t14_delay{
    uint32_t delay1;
    uint32_t delay2;
};

struct delay_measurement_info {
    char head[HEADLENGTH];
    struct rhub_data_delay* rhub_delay_up;
    struct rhub_data_delay* rhub_delay_down;
    struct rhub_t14_delay* rhub_t14;
};


class HUB :public uv::TcpClient
{
public:
    HUB(uv::EventLoop* loop);

    void onConnect(ConnectStatus status);
    void reConnect();
    void SendConnectMessage();
    void connectToServer(uv::SocketAddr& addr);
    void RecvMessage(const char* buf, ssize_t size);

    void SetRHUBInfo();
    void SendRHUBDelayInfo();

    void ConnectResultProcess(uv::PacketIR& packet);


    void SendMessage(const char* buf, ssize_t size);

    void TestProcess(uv::PacketIR& packet);
    void TestGetRhubDelay(uint8_t dir, struct rhub_data_delay* rhub_delay);
    void TestGetRhubT14Delay(struct rhub_t14_delay* t14_delay);

private:
    std::shared_ptr<uv::SocketAddr> sockAddr;
    std::shared_ptr<HUB> clientptr_;
    std::string m_source;
    std::string m_rruid;
    std::string m_port;
    std::string m_uport;
};

