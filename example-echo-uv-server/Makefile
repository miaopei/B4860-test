WORKDIR = `pwd`

CC = g++

CFLAGS = -Wall -std=c++11
INC = -I../
LIB = -luv
LDFLAGS = -pthread 

SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

EXECUTABLE = bbu_server

OBJDIR = $(EXECUTABLE)/obj
EXECDIR = $(EXECUTABLE)/bin

all: 
	g++ -lpthread -o tcp-client tcp-client.cpp -luv
	g++ -lpthread -o tcp-server tcp-server.cpp -luv

.PHONY:clean
clean:
	rm -rf tcp-client tcp-server

