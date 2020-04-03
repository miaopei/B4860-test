WORKDIR = `pwd`

CC = g++

CFLAGS = -Wall -std=c++11
INC = -I../../
LIB = -luv
LDFLAGS = -pthread 

SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

EXECUTABLE = main

OBJDIR = $(EXECUTABLE)/obj
EXECDIR = $(EXECUTABLE)/bin

all: before $(SOURCES) $(EXECUTABLE)
		
before:
	test -d $(OBJDIR) || mkdir -p $(OBJDIR)
	test -d $(EXECDIR) || mkdir -p $(EXECDIR)
	cp ../../uv/obj/* $(OBJDIR)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(wildcard $(OBJDIR)/*.o) $(INC) $(LIB) -o $(EXECDIR)/$@

.cpp.o:
	$(CC) -c $(CFLAGS) $(INC) $(LIB) $< -o $(OBJDIR)/$@

.PHONY:clean
clean:
	rm -rf $(EXECUTABLE)

