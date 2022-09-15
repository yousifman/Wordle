#Makefile for Project 3
CC = gcc
CFLAGS = -Wall -std=c99 -g

#target: wordle executable
wordle: wordle.o history.o lexicon.o io.o
	$(CC) $(CFLAGS) wordle.o history.o lexicon.o io.o -o wordle
wordle.o: history.h io.h lexicon.h
history.o: history.h
lexicon.o: lexicon.h io.h
io.o: io.h


clean: 
	rm -f *.o
	rm wordle
	rm history
	rm output.txt
