CC = gcc
CFLAGS = -W -Wall -g
LDFLAGS =

SRC = $(wildcard *.c)
OBJS = $(SRC:.c=.o)

all: PFatFs

PFatFs: $(OBJS) -lbcm2835
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@rm *.o
