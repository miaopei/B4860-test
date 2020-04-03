PWD=`pwd`

WORK_ROOT = $(PWD)

BUILD_DIR = $(WORK_ROOT)/../build
INCLUDE_DIR = $(BUILD_DIR)/include
BBU_INCLUDE_DIR = $(INCLUDE_DIR)/bbu
LIB_DIR = $(BUILD_DIR)/lib

TARGET = libbbu

CC = g++
SOLINK.cc = $(CC) -shared -o

CFLAGS = -Wall -std=c++11 -fPIC
USER_LINK_LIB = -lpthread -luv -L$(LIB_DIR)
USER_LINK_INC = -I$(INCLUDE_DIR) -I./include
CC_ARG = $(CFLAGS) $(USER_LINK_INC) $(USER_LINK_LIB)
LINK_ARG = $(CFLAGS) $(USER_LINK_INC) $(USER_LINK_LIB)

SOURCE = $(wildcard *.cpp)
OBJECTS = $(SOURCE:.cpp=.o)

all: $(TARGET).so
	@echo "[build libbbu ...]"
	@mv $(TARGET).so $(LIB_DIR)
	@test -d $(BBU_INCLUDE_DIR) || mkdir -p $(BBU_INCLUDE_DIR)
	@cp -rf include/* $(BBU_INCLUDE_DIR)

$(TARGET).so: $(OBJECTS)
	$(SOLINK.cc) $(TARGET).so $(OBJECTS) $(LINK_ARG)
	chmod a+rx $(TARGET).so

%.o: %.cpp
	$(CC) -c $(CFLAGS) $(USER_LINK_INC) $< -o $@ $(USER_LINK_LIB)

.PHONY:clean
clean:
	rm -rf *.o

