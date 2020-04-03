TARGET = bbu_server
WORKDIR = ./

CC = g++
CFLAGS = -Wall -std=c++11
INC = -I../src/
LIB = -luv
LDFLAGS = -pthread 

OBJDIR = $(WORKDIR)/$(TARGET)/obj
TARDIR = $(WORKDIR)/$(TARGET)/bin

TAG = $(TARDIR)/$(TARGET)

SRC1 = $(wildcard ../src/*.cpp)
SRC2 = $(wildcard ./*.cpp)
OBJ1 = $(patsubst %.cpp, $(OBJDIR)/%.o, $(notdir $(SRC1)))
OBJ2 = $(patsubst %.cpp, $(OBJDIR)/%.o, $(notdir $(SRC2)))
SRC = ../src/

all: before $(TAG)
		
before:
	test -d $(OBJDIR) || mkdir -p $(OBJDIR)
	test -d $(TARDIR) || mkdir -p $(TARDIR)

$(TAG): $(OBJ1) $(OBJ2)
	$(CC) $(CFLAGS) $(LDFLAGS) $(INC) $^ -o $@ $(LIB) 

$(OBJDIR)/%.o: $(SRC)%.cpp
	$(CC) -c $(CFLAGS) $(INC) $< -o $@ $(LIB) 

$(OBJDIR)/%.o: %.cpp
	$(CC) -c $(CFLAGS) $(INC) $< -o $@ $(LIB) 

.PHONY:clean
clean:
	rm -rf $(TARGET)

