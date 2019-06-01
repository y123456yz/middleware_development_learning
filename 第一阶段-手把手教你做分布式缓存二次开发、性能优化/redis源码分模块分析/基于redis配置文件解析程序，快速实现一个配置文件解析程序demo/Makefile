TARGET = config
OBJS = zmalloc.o sds.o config.o  main.o
DEPS = sds.h config.h zmalloc.h 
CPPFLAGS = -g
CC = gcc

$(TARGET) : $(OBJS) 
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

sds.o: sds.c
config.o : config.c
zmalloc.o: zmalloc.c
main.o : main.c $(DEPS)

clean :
	rm $(OBJS) $(TARGET)
