all: main

main: main.o A4.o
	g++ -W -Wall -o main main.o A4.o

main.o: main.cpp A4.h
	g++ -c main.cpp

A4.o: A4.cpp A4.h
	g++ -c A4.cpp

.PHONY: clean
clean: 
	rm main A4.o main.o