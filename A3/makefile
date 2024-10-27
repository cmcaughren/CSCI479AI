all: main

main: main.o A3.o
	g++ -W -Wall -o main main.o A3.o

main.o: main.cpp A3.h
	g++ -c main.cpp

A3.o: A3.cpp A3.h
	g++ -c A3.cpp

.PHONY: clean
clean: 
	rm A3.o main.o