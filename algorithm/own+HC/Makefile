CC=g++

CFLAGS=-c -Wall -g -ggdb

seed=1
iterations1=10
iterations2=10
iterations3=10
instance=../test/test

all: main

main: main.o Individual.o PatientsData.o
	$(CC) main.o -o main Individual.o PatientsData.o

PatientsData.o: PatientsData.cpp
	$(CC) $(CFLAGS) PatientsData.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

Individual.o: Individual.cpp
	$(CC) $(CFLAGS) Individual.cpp

clean:
	rm -rf *o main core

exe:
	./main $(seed) $(iterations1) $(iterations2) $(iterations3) < $(instance) 