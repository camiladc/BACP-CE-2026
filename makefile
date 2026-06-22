CC=g++
LD=g++
RM=rm -f

CFLAGS=-Wall -ansi -pedantic -g -std=c++20
LDFLAGS=-std=c++20
OBJS:=$(patsubst %.cpp,%.o,$(wildcard *.cpp))
MAIN=main
all:$(MAIN)
$(MAIN):$(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(MAIN) -lm
%.o: %.cpp global.h
	$(CC) $(CFLAGS) -c $<
clean:
	$(RM) $(OBJS)
