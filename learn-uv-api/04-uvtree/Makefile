TARGET = uvtree

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
	$(CC) $(CFLAGS) $(LIB) $(LDFLAGS) -o $(TARGET) ./$(OBJDIR)/*.o
		
$(OBJS):%.o:%.c
	$(CC) $(CFLAGS) $(LIB) $(LDFLAGS) -c $< $(INCLUDE) -o ./$(OBJDIR)/$@

.PHONY:clean
clean:
	rm -rf $(TARGET) $(OBJDIR)

