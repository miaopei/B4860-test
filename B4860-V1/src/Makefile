WORKDIR = `pwd`

CC = g++

CFLAGS = -Wall -std=c++11
LDFLAGS = -pthread 

SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

EXECUTABLE = uv

OBJDIR = ../$(EXECUTABLE)/obj
EXECDIR = ../$(EXECUTABLE)/bin

all: before $(SOURCES) $(EXECUTABLE)
		
before:
	test -d $(OBJDIR) || mkdir -p $(OBJDIR)
	test -d $(EXECDIR) || mkdir -p $(EXECDIR)
	cp -rf include ../$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	#$(CC) $(LDFLAGS) $(wildcard $(OBJDIR)/*.o) -o $(EXECDIR)/$@

.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $(OBJDIR)/$@

.PHONY:clean
clean:
	rm -rf ../$(EXECUTABLE)

