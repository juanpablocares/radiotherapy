CC=g++

CFLAGS=-c -Wall -g -ggdb

seed=1
instance=profe.rad

all: main

main: main.o
	$(CC) main.o -o main

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

clean:
	rm -rf *o main core

exe:
	./main $(seed) < $(instance)