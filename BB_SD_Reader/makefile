CC = gcc
CFLAGS = -W -Wall -g
LDFLAGS =

SRC = $(wildcard *.c)
OBJS = $(SRC:.c=.o)

all: SD_Reader

SD_Reader: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@rm *.o


