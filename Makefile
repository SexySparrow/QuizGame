#!/bin/bash

FLAG=-lncurses -lmenu

build: trivia.c
	gcc trivia.c -Wall -o trivia $(FLAG)
run: trivia
	./trivia Data/GameData.txt
clean:
	rm trivia
