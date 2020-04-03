
# 源文件目录
AUX_SOURCE_DIRECTORY(. DIR_WORLD_SRCS)

# 设置环境变量，编译用到的源文件全部都要放到这里，否则编译能够通过，但是执行的时候会出现各种问题，比如"symbol lookup error xxxxx , undefined symbol"
SET(WORLD_SOURCES ${DIR_WORLD_SRCS})

MESSAGE("building world...")

ADD_LIBRARY(world ${WORLD_SOURCES})

