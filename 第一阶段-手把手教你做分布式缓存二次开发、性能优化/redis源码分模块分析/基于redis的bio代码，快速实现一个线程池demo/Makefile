TARGET = threadPoll
OBJS = adlist.o bio.o zmalloc.o threadPoll_Main.o
DEPS = adlist.h bio.h zmalloc.h
CPPFLAGS = -g
CC = gcc
LIBLINK = -lpthread

$(TARGET) : $(OBJS) 
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS) $(LIBLINK)

adlist.o : adlist.c
bio.o: bio.c
zmalloc.o: zmalloc.c
threadPoll_Main.o : threadPoll_Main.c $(DEPS)

clean :
	rm $(OBJS) $(TARGET)
