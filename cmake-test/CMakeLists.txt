cmake_minimum_required(VERSION 3.5.1)

# 添加c++11标准支持
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -O2")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")

PROJECT(helloworld)

# 头文件目录
INCLUDE_DIRECTORIES(
./
./hello
./world
)

# 源文件目录
AUX_SOURCE_DIRECTORY(. DIR_SRCS)

# 设置环境变量，编译用到的源文件全部都要放到这里，否则编译能够通过，但是执行的时候会出现各种问题，比如"symbol lookup error xxxxx , undefined symbol"
SET(SOURCES ${DIR_SRCS})

ADD_SUBDIRECTORY(hello)
ADD_SUBDIRECTORY(world)

MESSAGE("building...")

# 添加要编译的可执行文件
ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCES})

#ADD_LIBRARY (${PROJECT_NAME} STATIC ${SOURCES})

# 添加可执行文件所需要的库，比如我们用到了libm.so（命名规则：lib+name+.so），就添加该库的名称
TARGET_LINK_LIBRARIES(${PROJECT_NAME} hello world)

