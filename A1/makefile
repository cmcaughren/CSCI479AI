all: main

main: main.o A1.o
	g++ -W -Wall -o main main.o A1.o

main.o: main.cpp A1.h
	g++ -c main.cpp

A1.o: A1.cpp A1.h
	g++ -c A1.cpp

.PHONY: clean
clean: 
	rm A1.o main.o