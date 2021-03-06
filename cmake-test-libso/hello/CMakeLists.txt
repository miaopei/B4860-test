#cmake_minimum_required(VERSION 3.5.1)

# 添加c++11标准支持
#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -O2")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")

# 源文件目录
AUX_SOURCE_DIRECTORY(. DIR_SRCS)

# 设置环境变量，编译用到的源文件全部都要放到这里，否则编译能够通过，但是执行的时候会出现各种问题，比如"symbol lookup error xxxxx , undefined symbol"
SET(SOURCES ${DIR_SRCS})

# three kinds of libraries:
# 1. shared, i.e., dynamic library，动态库文件扩展名常为 "*.so"；
# 2. static, i.e., static library， 静态库文件扩展名常为 "*.a";
# 3. module, this parameter is valid only when dyld is supported;
# otherwise, will be considered as shared

# 添加动态库，关键词为shared，你不需要写全libhello.so，
# 只需要填写hello即可，cmake系统会自动为你生成 libhello.X
ADD_LIBRARY (hello SHARED ${SOURCES})

# 添加静态库，关键词为static，
# ADD_LIBRARY (hello STATIC ${SOURCES})
# 仍然用hello作为target名时，是不能成功创建所需的静态库的，
# 因为hello作为一个target是不能重名的， 故把上面的hello修改为hello_static
# 同理，你不需要写全libhello_static.a
# 只需要填写hello即可，cmake系统会自动为你生成 libhello_static.X
ADD_LIBRARY (hello_static STATIC ${SOURCES})

# 按照一般的习惯，静态库名字跟动态库名字应该是一致的，只是扩展名不同；
# 即：静态库名为 libhello.a； 动态库名为libhello.so ；
# 所以，希望 "hello_static" 在输出时，不是"hello_static"，而是以"hello"的名字显示，故设置如下：
# SET_TARGET_PROPERTIES (target1 target2 ...PROPERTIES prop1 value1 prop2 value2 ...)
# GET_TARGET_PROPERTY (VAR target property)
SET_TARGET_PROPERTIES (hello_static PROPERTIES OUTPUT_NAME "hello")
GET_TARGET_PROPERTY (OUTPUT_VALUE hello_static OUTPUT_NAME)
MESSAGE (STATUS "This is the hello_static OUTPUT_NAME: " ${OUTPUT_VALUE})

# cmake在构建一个新的target时，会尝试清理掉其他使用这个名字的库，
# 因此，在构建libhello.a时，就会清理掉libhello.so.
# 为了回避这个问题，比如再次使用SET_TARGET_PROPERTIES定义 CLEAN_DIRECT_OUTPUT属性。
SET_TARGET_PROPERTIES (hello_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES (hello PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# 按照规则，动态库是应该包含一个版本号的，
# VERSION指代动态库版本，SOVERSION指代API版本。
SET_TARGET_PROPERTIES (hello PROPERTIES VERSION 1.2 SOVERSION 1)

# 我们需要将libhello.a, libhello.so.x以及hello.h安装到系统目录，才能真正让其他人开发使用，
# 在本例中我们将hello的共享库安装到<prefix>/lib目录；
# 将hello.h安装<prefix>/include/hello目录。
INSTALL (TARGETS hello hello_static LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)

INSTALL (FILES hello.h DESTINATION include/hello)

# 通过:
# cmake -DCMAKE_INSTALL_PREFIX=./ ..
# make
# sudo make install
# 我们就可以将头文件和共享库安装到 ./build/src/lib和 ./build/src/include/hello中了。

