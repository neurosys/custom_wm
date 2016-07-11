
CC=gcc
PROJECT=barM
CFLAGS=-W -Wall -c
LDFLAGS=-lXft -lX11
OBJS=barM.o

all: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(PROJECT)

barM.o: barM.c
	$(CC) $(CFLAGS) barM.c -o barM.o

clean:
	rm -f $(PROJECT)
	rm -f *.o
