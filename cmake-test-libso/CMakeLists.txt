cmake_minimum_required(VERSION 3.5.1)

# 添加c++11标准支持
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -O2")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")

PROJECT(HELLOLIB)

# 通过在主工程文件CMakeLists.txt中修改ADD_SUBDIRECTORY (lib) 指令来指定一个编译输出位置;
ADD_SUBDIRECTORY(hello)

