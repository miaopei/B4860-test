#include <iostream>
#include <stdint.h>
#include <string>
#include <sstream>

#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <map>
#include "test.h"
#include "PacketIR.h"
#include "Connect.h"

using namespace std;


string dec2hex2(int dec, size_t length)
{
    string s_temp; //存放转化后字符
    
    int i;
    for (i = length - 1; i >= 0; i--)
    {
        s_temp[i] = (dec % 256) & 0xFF;
        dec /= 256;

    }
    return s_temp;
}

string toHex(int num) {
    const string HEX = "0123456789abcdef";
    if (num == 0) return "0";
    string result;
    int count = 0;
    while (num && count++ < 8) {
        result = HEX[(num & 0xf)] + result;
        num >>= 4;
    }
    return result;
}

void DectoHex(int dec, unsigned char *output, int length)  
{  
    const char * hex = "0123456789ABCDEF";
    for (int i = 0; i<length; i++)  
    {  
        output[length - i -1] = hex[(dec >> i * 4) & 0x0F]; 
    } 
} 

void dec2char(int dec, char* buf, size_t size)
{
    sprintf(buf, "%d", dec);
}

int main()
{
#if 0
    test::Data testData = {"Mr.Miaow", 30};
    
    //test << Data;

    std::ostringstream stream;
    stream << testData;
    const std::string serializedData = stream.str();
    std::cout << "serializedData=" << serializedData << std::endl;


    std::string dx = "11001x";
    std::cout << "type=" << dx.substr(0, 1) << std::endl;
    std::cout << "msgID=" << dx.substr(1, 4) << std::endl;
#endif

// 封装 packet
#if 0
    std::string data = "key=value&key2=value2";
    uv::PacketIR packet;
    //uv::PacketIR packet("1", "1666");
    if(packet.SetHead("1", "1888"))
    {
        packet.PackMessage(data, data.length());
        std::cout << "封装 packet:" << std::endl;
        std::cout << "\tGetPacket: " << packet.GetPacket() << std::endl;
        std::cout << "\tGetHead: " << packet.GetHead() << std::endl;
        std::cout << "\tGetType: " << packet.GetType() << std::endl;
        std::cout << "\tGetMsgID: " << packet.GetMsgID() << std::endl;
        std::cout << "\tGetData: " << packet.GetData() << std::endl;
    }
#endif

// 解析 packet
#if 0
    std::string data = "11666key=value&key2=value2&key3=value3";

    uv::PacketIR packet;
    packet.UnPackMessage(data);

    std::cout << "解析 packet:" << std::endl; 
    std::cout << "\tGetPacket: " << packet.GetPacket() << std::endl;
    std::cout << "\tGetHead: " << packet.GetHead() << std::endl;
    std::cout << "\tGetType: " << packet.GetType() << std::endl;
    std::cout << "\tGetMsgID: " << packet.GetMsgID() << std::endl;
    std::cout << "\tGetData: " << packet.GetData() << std::endl;

#endif

#if 0
    enum test
    {
        TEST_0 = 0,
        TEST_1 = 1,
        TEST_2 = 2
    };

    std::string tostring;
    std::cout << "tostring=" << to_string(TEST_0) << std::endl;
    std::cout << "tostring=" << to_string(TEST_1) << std::endl;
    std::cout << "tostring=" << to_string(TEST_2) << std::endl;
#endif

#if 0
    Connect::ClientInfo s;

    std::map<std::string, Connect::ClientInfo> ConnectInfo;

    ConnectInfo.insert(pair<std::string, Connect::ClientInfo>("10", Connect::ClientInfo("1", "1040", "5")));
    ConnectInfo.insert(pair<std::string, Connect::ClientInfo>("11", Connect::ClientInfo("1", "1041", "6")));
    ConnectInfo.insert(pair<std::string, Connect::ClientInfo>("12", Connect::ClientInfo("1", "1042", "7")));
    ConnectInfo.insert(pair<std::string, Connect::ClientInfo>("13", s));
    s.s_type = "1";
    s.s_RRUID = "1043";
    s.s_port = "8";
    //ConnectInfo.insert(pair<std::string, Connect::ClientInfo>("13", s));
    ConnectInfo["13"] = s;
#endif

#if 0
    for (auto it = ConnectInfo.begin(); it != ConnectInfo.end(); ++it) {
        std::cout << it->first << " - > " 
            << it->second.s_type << " " 
            << it->second.s_RRUID << " " 
            << it->second.s_port <<std::endl;
    }
#endif

#if 0
    for(auto &it : ConnectInfo)
    {
         std::cout << it.first << " - > " 
            << it.second.s_type << " " 
            << it.second.s_RRUID << " " 
            << it.second.s_port <<std::endl;
    }
#endif

#if 0
    std::string testlength = "0021";

    std::cout << std::stoi(testlength) << std::endl;

    int i = 21211;                     
    char ss[3];                    
    sprintf(ss, "%04d", i);         

    std::cout << "ss=" << ss << std::endl;
    std::cout << "size-ss=" << strlen(ss) << std::endl;
#endif

#if 0
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

    std::cout << "s2d = " << (enum Destination)OAM << std::endl;

    std::cout << "enum2string: " << Source(RRU) << std::endl;
#endif

#if 0
    struct Test
    {
        int a;
        char b;
        char c[1024];
        unsigned short d;

    };

    char* ch = "abcdefghijklmn1234567890";
    char* chTest = new char[2048];
    memset(chTest, 0, sizeof(chTest));
     
    Test* t1 = new Test;
    t1->a = 10;
    t1->b = -20;
    strcpy(t1->c, ch);
    t1->d = 12;
     
    //结构体转为char*
    memcpy(chTest, (char*)t1, sizeof(Test));

    std::cout << "chTest=" << chTest << std::endl;
    
    //char*转为结构体
    Test* t2;
    t2 = (Test*)chTest;

    std::cout << "t2.a=" << t2->a << std::endl;
    std::cout << "t2.c=" << t2->c << std::endl;
#endif

#if 1
    std::string data = "delay1_up=24&delay2_up=25&delay1_down=26&delay2_down=27&t14_delay1=11456&t14_delay2=11457";


#endif


    return 0;
}

