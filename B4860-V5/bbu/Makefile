PWD=`pwd`

WORK_ROOT = $(PWD)

BUILD_DIR = $(WORK_ROOT)/../build
INCLUDE_DIR = $(BUILD_DIR)/include/bbu
LIB_DIR = $(BUILD_DIR)/lib
BIN_DIR = $(BUILD_DIR)/bin

TARGET = bbu

CC = g++
CFLAGS = -Wall -std=c++11 
USER_LINK_INC = -I$(INCLUDE_DIR)
USER_LINK_LIB = -L$(LIB_DIR) -pthread -luv -lbbu -Wl,-rpath,$(LIB_DIR)

SOURCE = $(wildcard *.cpp)
OBJECTS = $(SOURCE:.cpp=.o)

all: $(TARGET)
	@echo "[build bbu ...]"		
	@mv $(TARGET) $(BIN_DIR)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(USER_LINK_INC) $^ -o $@ $(USER_LINK_LIB) 

%.o: %.cpp
	$(CC) -c $(CFLAGS) $(USER_LINK_INC) $< -o $@ $(USER_LINK_LIB) 

.PHONY:clean
clean:
	@echo "[Cleanning bbu ...]"
	@rm -rf $(OBJECTS)

