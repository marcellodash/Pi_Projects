all: DF_Player

DF_Player: main.o
	gcc main.o -lwiringPi -o DF_Player

main.o: main.c
	gcc -c main.c

clean:
	rm -rf *o DF_Player
