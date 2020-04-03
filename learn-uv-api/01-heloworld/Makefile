TARGET = helloworld

CC = gcc
CFLAGS = -Wall 
INC = -I../
LIB = -luv 
LDFLAGS = -pthread 

OBJDIR = ./obj
OBJS = $(patsubst %.c, %.o, $(shell ls *.c))

all: before $(TARGET)
	
before:
	test -d $(OBJDIR) || mkdir -p $(OBJDIR)

$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) ./$(OBJDIR)/*.o $(LIB)
		
$(OBJS):%.o:%.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< $(INCLUDE) -o ./$(OBJDIR)/$@ $(LIB) 

.PHONY:clean
clean:
	rm -rf $(TARGET) $(OBJDIR)

