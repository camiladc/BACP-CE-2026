CC=g++
LD=g++
RM=rm -f

CFLAGS=-Wall -ansi -pedantic -g
OBJS:=$(patsubst %.cpp,%.o,$(wildcard *.cpp))
MAIN=main
all:$(MAIN)
$(MAIN):$(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(MAIN) -lm
%.o: %.c global.h
	$(CC) $(CFLAGS) -c $<
clean:
	$(RM) $(OBJS)
