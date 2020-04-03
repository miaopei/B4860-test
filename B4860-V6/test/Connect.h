#pragma once

#include <iostream>

class Connect
{
public:
    struct ClientInfo
    {
        std::string s_type;
        std::string s_RRUID;
        std::string s_port;
        ClientInfo(){};
        ClientInfo(std::string type, std::string rruid, std::string port)
        {
            s_type = type;
            s_RRUID = rruid;
            s_port = port;
        };
    };

    Connect() {}
    ~Connect() {}

private:

};

