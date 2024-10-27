all: main

main: main.o A2.o
	g++ -W -Wall -o main main.o A2.o

main.o: main.cpp A2.h
	g++ -c main.cpp

A2.o: A2.cpp A2.h
	g++ -c A2.cpp

.PHONY: clean
clean: 
	rm A2.o main.o