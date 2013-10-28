CC=g++

CFLAGS=-c -Wall -g -ggdb

seed=1
iterations=100
restarts=1
p_insert_eliminate=0.1
p_eme=0.5
p_pal=0.5
p_rad=0.5
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
	./main $(seed) $(iterations) $(restarts) $(p_insert_eliminate) $(p_eme) $(p_pal) $(p_rad) < $(instance)