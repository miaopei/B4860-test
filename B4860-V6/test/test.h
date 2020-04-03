#pragma once

#include <iostream>
#include <string>
#include <sstream>

class test
{
    std::ostringstream os_;
public:
    struct Data
    {  
        std::string name;
        int age;
        std::string GetName(){return name;}
        int GetAge(){return age;}
    };

    test() {}
    ~test() {}

#if 0
    std::ostringstream &operator<<( std::ostringstream &stream, const Data &v  ) {
          return stream << v.name << v.age;
    }

    std::ostringstream &operator>>( std::ostringstream &stream, Data &v  ) {
          return stream >> v.length; >> v.message;
          
    }
#endif

    friend std::ostream& operator<<(std::ostream& os, const Data& data) {
        return os << data.name << data.age;
    }

private:

};


